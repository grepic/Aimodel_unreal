// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Voice/VoiceSynthesisManager.h"
#include "AIDigitalHumanModule.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundWaveProcedural.h"
#include "Engine/World.h"
#include "TimerManager.h"

UVoiceSynthesisManager::UVoiceSynthesisManager()
{
    // Default voice configuration
    VoiceConfig.VoiceID = TEXT("nova-female-1");
    VoiceConfig.SpeakingRate = 1.0f;
    VoiceConfig.Pitch = 1.0f;
    VoiceConfig.Volume = 1.0f;
    VoiceConfig.bEnableBreathing = true;
    VoiceConfig.bEmotionModulation = true;
}

void UVoiceSynthesisManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing Voice Synthesis Manager..."));

    bIsInitialized = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Voice Synthesis Manager Initialized"));
}

void UVoiceSynthesisManager::Speak(const FString& Text, EDigitalHumanEmotion Emotion)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Voice Synthesis Manager not initialized"));
        return;
    }

    if (Text.IsEmpty())
    {
        return;
    }

    // If already speaking, queue this text
    if (IsSpeaking())
    {
        QueueSpeech(Text, Emotion);
        return;
    }

    // Generate speech
    GenerateSpeech(Text, Emotion);
}

void UVoiceSynthesisManager::GenerateSpeech(const FString& Text, EDigitalHumanEmotion Emotion)
{
    // Preprocess text
    FString ProcessedText = PreprocessText(Text);

    // Get emotion-modulated voice config
    FVoiceConfiguration ModulatedConfig = GetEmotionModulatedConfig(Emotion);

    // Update playback state
    PlaybackState.bIsPlaying = true;
    PlaybackState.CurrentTime = 0.0f;
    PlaybackState.CurrentText = Text;
    PlaybackState.CurrentEmotion = Emotion;

    if (TTSEndpoint.IsEmpty())
    {
        // Fallback: Generate placeholder lip sync for testing
        UE_LOG(LogAIDigitalHuman, Log, TEXT("TTS not configured, generating test lip sync data"));

        // Estimate duration based on text length (rough estimate: 100ms per character)
        float EstimatedDuration = ProcessedText.Len() * 0.1f;
        PlaybackState.TotalDuration = EstimatedDuration;

        // Generate test lip sync data
        CurrentLipSyncData.Duration = EstimatedDuration;
        CurrentLipSyncData.Frames.Empty();

        // Generate placeholder frames
        int32 FrameCount = FMath::CeilToInt(EstimatedDuration * 60.0f);
        for (int32 i = 0; i < FrameCount; i++)
        {
            FLipSyncFrame Frame;
            Frame.Timestamp = i / 60.0f;
            Frame.Phoneme = static_cast<EPhoneme>(FMath::RandRange(0, 10));
            Frame.Intensity = FMath::Sin(i * 0.3f) * 0.5f + 0.5f;
            Frame.BlendshapeWeights = PhonemeToBlendshapes(Frame.Phoneme, Frame.Intensity);
            CurrentLipSyncData.Frames.Add(Frame);
        }

        CurrentLipSyncFrameIndex = 0;
        OnLipSyncDataReady.Broadcast(CurrentLipSyncData);

        // Simulate playback completion after duration
        if (UWorld* World = GetWorld())
        {
            FTimerHandle CompletionTimer;
            World->GetTimerManager().SetTimer(
                CompletionTimer,
                [this]()
                {
                    PlaybackState.bIsPlaying = false;
                    OnPlaybackComplete.Broadcast();
                    ProcessSpeechQueue();
                },
                EstimatedDuration,
                false
            );

            // Start lip sync updates
            World->GetTimerManager().SetTimer(
                LipSyncUpdateTimer,
                [this]() { UpdateLipSync(1.0f / 60.0f); },
                1.0f / 60.0f,
                true
            );
        }

        return;
    }

    // Build TTS API request
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(TTSEndpoint);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *TTSAPIKey));

    // Build JSON payload
    FString RequestBody = FString::Printf(TEXT(R"({
        "text": "%s",
        "voice": "%s",
        "speed": %.2f,
        "pitch": %.2f,
        "output_format": "wav",
        "sample_rate": 44100
    })"),
        *ProcessedText.ReplaceCharWithEscapedChar(),
        *ModulatedConfig.VoiceID,
        ModulatedConfig.SpeakingRate,
        ModulatedConfig.Pitch
    );

    Request->SetContentAsString(RequestBody);
    Request->OnProcessRequestComplete().BindUObject(this, &UVoiceSynthesisManager::OnTTSResponseReceived);
    Request->ProcessRequest();
}

void UVoiceSynthesisManager::OnTTSResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (!bSuccess || !Response.IsValid())
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("TTS API request failed"));
        OnError.Broadcast(TEXT("Failed to generate speech"));
        PlaybackState.bIsPlaying = false;
        ProcessSpeechQueue();
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    if (ResponseCode != 200)
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("TTS API returned error: %d"), ResponseCode);
        OnError.Broadcast(FString::Printf(TEXT("TTS service error: %d"), ResponseCode));
        PlaybackState.bIsPlaying = false;
        ProcessSpeechQueue();
        return;
    }

    // Get audio data
    TArray<uint8> AudioData = Response->GetContent();

    // Generate lip sync data from audio
    CurrentLipSyncData = GenerateLipSyncData(AudioData, 44100);
    CurrentLipSyncFrameIndex = 0;

    OnLipSyncDataReady.Broadcast(CurrentLipSyncData);

    // Create sound wave from audio data
    // Note: Actual implementation would use USoundWaveProcedural or similar

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Speech synthesis complete, duration: %.2fs"), CurrentLipSyncData.Duration);
}

FLipSyncData UVoiceSynthesisManager::GenerateLipSyncData(const TArray<uint8>& AudioData, int32 SampleRate)
{
    FLipSyncData LipSync;
    LipSync.SampleRate = 60; // 60fps lip sync

    // Calculate duration from audio data (assuming 16-bit PCM)
    int32 NumSamples = AudioData.Num() / 2;
    LipSync.Duration = static_cast<float>(NumSamples) / SampleRate;

    // Process audio in frames
    int32 SamplesPerFrame = SampleRate / LipSync.SampleRate;
    int32 FrameCount = NumSamples / SamplesPerFrame;

    const int16* Samples = reinterpret_cast<const int16*>(AudioData.GetData());

    for (int32 Frame = 0; Frame < FrameCount; Frame++)
    {
        FLipSyncFrame LipFrame;
        LipFrame.Timestamp = Frame / static_cast<float>(LipSync.SampleRate);

        // Analyze frame audio
        float MaxAmplitude = 0.0f;
        float AverageFrequency = 0.0f;

        int32 FrameStart = Frame * SamplesPerFrame;
        for (int32 s = 0; s < SamplesPerFrame && (FrameStart + s) < NumSamples; s++)
        {
            float Sample = Samples[FrameStart + s] / 32768.0f;
            MaxAmplitude = FMath::Max(MaxAmplitude, FMath::Abs(Sample));
        }

        // Determine phoneme based on audio characteristics
        LipFrame.Phoneme = AnalyzePhoneme(MaxAmplitude, AverageFrequency);
        LipFrame.Intensity = MaxAmplitude;
        LipFrame.BlendshapeWeights = PhonemeToBlendshapes(LipFrame.Phoneme, LipFrame.Intensity);

        LipSync.Frames.Add(LipFrame);
    }

    return LipSync;
}

EPhoneme UVoiceSynthesisManager::AnalyzePhoneme(float AudioAmplitude, float Frequency) const
{
    // Simplified phoneme detection based on amplitude
    // Real implementation would use proper audio analysis (formant detection, etc.)

    if (AudioAmplitude < 0.05f)
    {
        return EPhoneme::Silence;
    }
    else if (AudioAmplitude < 0.2f)
    {
        // Low amplitude - typically consonants like M, N, or reduced vowels
        return EPhoneme::M;
    }
    else if (AudioAmplitude < 0.4f)
    {
        // Medium amplitude - mid vowels
        return EPhoneme::EH;
    }
    else if (AudioAmplitude < 0.6f)
    {
        // Higher amplitude - open vowels
        return EPhoneme::AH;
    }
    else
    {
        // Maximum amplitude - fully open mouth
        return EPhoneme::AA;
    }
}

TMap<FName, float> UVoiceSynthesisManager::PhonemeToBlendshapes(EPhoneme Phoneme, float Intensity) const
{
    TMap<FName, float> Blendshapes;

    // Initialize all blendshapes to 0
    Blendshapes.Add(FName("jawOpen"), 0.0f);
    Blendshapes.Add(FName("mouthFunnel"), 0.0f);
    Blendshapes.Add(FName("mouthPucker"), 0.0f);
    Blendshapes.Add(FName("mouthLeft"), 0.0f);
    Blendshapes.Add(FName("mouthRight"), 0.0f);
    Blendshapes.Add(FName("mouthSmileLeft"), 0.0f);
    Blendshapes.Add(FName("mouthSmileRight"), 0.0f);
    Blendshapes.Add(FName("mouthStretchLeft"), 0.0f);
    Blendshapes.Add(FName("mouthStretchRight"), 0.0f);
    Blendshapes.Add(FName("mouthDimpleLeft"), 0.0f);
    Blendshapes.Add(FName("mouthDimpleRight"), 0.0f);
    Blendshapes.Add(FName("mouthUpperUpLeft"), 0.0f);
    Blendshapes.Add(FName("mouthUpperUpRight"), 0.0f);
    Blendshapes.Add(FName("mouthLowerDownLeft"), 0.0f);
    Blendshapes.Add(FName("mouthLowerDownRight"), 0.0f);
    Blendshapes.Add(FName("mouthPressLeft"), 0.0f);
    Blendshapes.Add(FName("mouthPressRight"), 0.0f);
    Blendshapes.Add(FName("mouthClose"), 0.0f);

    // Set blendshapes based on phoneme
    float I = Intensity;

    switch (Phoneme)
    {
    case EPhoneme::Silence:
        Blendshapes[FName("mouthClose")] = 0.3f * I;
        break;

    case EPhoneme::AA:
    case EPhoneme::AH:
        Blendshapes[FName("jawOpen")] = 0.7f * I;
        Blendshapes[FName("mouthLowerDownLeft")] = 0.4f * I;
        Blendshapes[FName("mouthLowerDownRight")] = 0.4f * I;
        break;

    case EPhoneme::AE:
        Blendshapes[FName("jawOpen")] = 0.5f * I;
        Blendshapes[FName("mouthStretchLeft")] = 0.3f * I;
        Blendshapes[FName("mouthStretchRight")] = 0.3f * I;
        break;

    case EPhoneme::EH:
    case EPhoneme::IH:
        Blendshapes[FName("jawOpen")] = 0.3f * I;
        Blendshapes[FName("mouthSmileLeft")] = 0.2f * I;
        Blendshapes[FName("mouthSmileRight")] = 0.2f * I;
        break;

    case EPhoneme::IY:
        Blendshapes[FName("jawOpen")] = 0.2f * I;
        Blendshapes[FName("mouthSmileLeft")] = 0.5f * I;
        Blendshapes[FName("mouthSmileRight")] = 0.5f * I;
        break;

    case EPhoneme::OW:
    case EPhoneme::UW:
        Blendshapes[FName("jawOpen")] = 0.3f * I;
        Blendshapes[FName("mouthFunnel")] = 0.6f * I;
        Blendshapes[FName("mouthPucker")] = 0.4f * I;
        break;

    case EPhoneme::M:
    case EPhoneme::B:
    case EPhoneme::P:
        Blendshapes[FName("mouthClose")] = 0.8f * I;
        Blendshapes[FName("mouthPressLeft")] = 0.4f * I;
        Blendshapes[FName("mouthPressRight")] = 0.4f * I;
        break;

    case EPhoneme::F:
    case EPhoneme::V:
        Blendshapes[FName("mouthClose")] = 0.3f * I;
        Blendshapes[FName("mouthUpperUpLeft")] = 0.3f * I;
        Blendshapes[FName("mouthUpperUpRight")] = 0.3f * I;
        break;

    case EPhoneme::TH:
    case EPhoneme::DH:
        Blendshapes[FName("jawOpen")] = 0.2f * I;
        // Tongue position would be handled separately
        break;

    case EPhoneme::S:
    case EPhoneme::Z:
        Blendshapes[FName("jawOpen")] = 0.1f * I;
        Blendshapes[FName("mouthSmileLeft")] = 0.1f * I;
        Blendshapes[FName("mouthSmileRight")] = 0.1f * I;
        break;

    case EPhoneme::SH:
    case EPhoneme::ZH:
    case EPhoneme::CH:
    case EPhoneme::JH:
        Blendshapes[FName("jawOpen")] = 0.15f * I;
        Blendshapes[FName("mouthFunnel")] = 0.4f * I;
        break;

    case EPhoneme::L:
        Blendshapes[FName("jawOpen")] = 0.25f * I;
        break;

    case EPhoneme::R:
        Blendshapes[FName("jawOpen")] = 0.2f * I;
        Blendshapes[FName("mouthFunnel")] = 0.2f * I;
        break;

    case EPhoneme::W:
        Blendshapes[FName("mouthPucker")] = 0.7f * I;
        Blendshapes[FName("mouthFunnel")] = 0.3f * I;
        break;

    case EPhoneme::Y:
        Blendshapes[FName("jawOpen")] = 0.2f * I;
        Blendshapes[FName("mouthSmileLeft")] = 0.3f * I;
        Blendshapes[FName("mouthSmileRight")] = 0.3f * I;
        break;

    default:
        Blendshapes[FName("jawOpen")] = 0.3f * I;
        break;
    }

    return Blendshapes;
}

void UVoiceSynthesisManager::UpdateLipSync(float DeltaTime)
{
    if (!PlaybackState.bIsPlaying || bIsPaused)
    {
        return;
    }

    PlaybackState.CurrentTime += DeltaTime;

    // Find current frame
    while (CurrentLipSyncFrameIndex < CurrentLipSyncData.Frames.Num() - 1)
    {
        if (CurrentLipSyncData.Frames[CurrentLipSyncFrameIndex + 1].Timestamp > PlaybackState.CurrentTime)
        {
            break;
        }
        CurrentLipSyncFrameIndex++;
    }

    // Broadcast current frame
    if (CurrentLipSyncFrameIndex < CurrentLipSyncData.Frames.Num())
    {
        OnPhonemeChanged.Broadcast(CurrentLipSyncData.Frames[CurrentLipSyncFrameIndex]);
    }

    // Check for completion
    if (PlaybackState.CurrentTime >= CurrentLipSyncData.Duration)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(LipSyncUpdateTimer);
        }
        PlaybackState.bIsPlaying = false;
        OnPlaybackComplete.Broadcast();
        ProcessSpeechQueue();
    }
}

void UVoiceSynthesisManager::StopSpeaking()
{
    if (AudioComponent)
    {
        AudioComponent->Stop();
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(LipSyncUpdateTimer);
    }

    PlaybackState.bIsPlaying = false;
    bIsPaused = false;
    SpeechQueue.Empty();

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Speech stopped"));
}

void UVoiceSynthesisManager::PauseSpeaking()
{
    if (!PlaybackState.bIsPlaying || bIsPaused)
    {
        return;
    }

    if (AudioComponent)
    {
        AudioComponent->SetPaused(true);
    }

    bIsPaused = true;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Speech paused"));
}

void UVoiceSynthesisManager::ResumeSpeaking()
{
    if (!bIsPaused)
    {
        return;
    }

    if (AudioComponent)
    {
        AudioComponent->SetPaused(false);
    }

    bIsPaused = false;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Speech resumed"));
}

void UVoiceSynthesisManager::SetVoiceConfiguration(const FVoiceConfiguration& Config)
{
    VoiceConfig = Config;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Voice configuration updated"));
}

void UVoiceSynthesisManager::SetTTSEndpoint(const FString& Endpoint, const FString& APIKey)
{
    TTSEndpoint = Endpoint;
    TTSAPIKey = APIKey;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("TTS endpoint configured: %s"), *Endpoint);
}

bool UVoiceSynthesisManager::IsSpeaking() const
{
    return PlaybackState.bIsPlaying && !bIsPaused;
}

FAudioPlaybackState UVoiceSynthesisManager::GetPlaybackState() const
{
    return PlaybackState;
}

FLipSyncFrame UVoiceSynthesisManager::GetCurrentLipSyncFrame() const
{
    if (CurrentLipSyncFrameIndex < CurrentLipSyncData.Frames.Num())
    {
        return CurrentLipSyncData.Frames[CurrentLipSyncFrameIndex];
    }

    FLipSyncFrame EmptyFrame;
    EmptyFrame.Phoneme = EPhoneme::Silence;
    return EmptyFrame;
}

TMap<FName, float> UVoiceSynthesisManager::GetCurrentBlendshapeWeights() const
{
    FLipSyncFrame CurrentFrame = GetCurrentLipSyncFrame();
    return CurrentFrame.BlendshapeWeights;
}

void UVoiceSynthesisManager::QueueSpeech(const FString& Text, EDigitalHumanEmotion Emotion)
{
    SpeechQueue.Add(TPair<FString, EDigitalHumanEmotion>(Text, Emotion));
    UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Speech queued, queue size: %d"), SpeechQueue.Num());
}

void UVoiceSynthesisManager::ProcessSpeechQueue()
{
    if (SpeechQueue.Num() > 0)
    {
        TPair<FString, EDigitalHumanEmotion> Next = SpeechQueue[0];
        SpeechQueue.RemoveAt(0);
        GenerateSpeech(Next.Key, Next.Value);
    }
}

FVoiceConfiguration UVoiceSynthesisManager::GetEmotionModulatedConfig(EDigitalHumanEmotion Emotion) const
{
    if (!VoiceConfig.bEmotionModulation)
    {
        return VoiceConfig;
    }

    FVoiceConfiguration Modulated = VoiceConfig;

    switch (Emotion)
    {
    case EDigitalHumanEmotion::Happy:
        Modulated.Pitch *= 1.05f;
        Modulated.SpeakingRate *= 1.05f;
        break;

    case EDigitalHumanEmotion::Excited:
        Modulated.Pitch *= 1.1f;
        Modulated.SpeakingRate *= 1.15f;
        Modulated.Volume *= 1.1f;
        break;

    case EDigitalHumanEmotion::Sad:
        Modulated.Pitch *= 0.95f;
        Modulated.SpeakingRate *= 0.9f;
        break;

    case EDigitalHumanEmotion::Thinking:
        Modulated.SpeakingRate *= 0.85f;
        break;

    case EDigitalHumanEmotion::Surprised:
        Modulated.Pitch *= 1.15f;
        Modulated.SpeakingRate *= 1.1f;
        break;

    case EDigitalHumanEmotion::Flirty:
        Modulated.Pitch *= 0.98f;
        Modulated.SpeakingRate *= 0.95f;
        break;

    default:
        break;
    }

    // Clamp values
    Modulated.Pitch = FMath::Clamp(Modulated.Pitch, 0.5f, 2.0f);
    Modulated.SpeakingRate = FMath::Clamp(Modulated.SpeakingRate, 0.5f, 2.0f);
    Modulated.Volume = FMath::Clamp(Modulated.Volume, 0.0f, 1.5f);

    return Modulated;
}

FString UVoiceSynthesisManager::PreprocessText(const FString& Text) const
{
    FString Processed = Text;

    // Add natural pauses at punctuation
    if (VoiceConfig.bEnableBreathing)
    {
        // SSML-like markers could be added here for TTS services that support it
    }

    // Clean up any unwanted characters
    Processed = Processed.TrimStartAndEnd();

    return Processed;
}
