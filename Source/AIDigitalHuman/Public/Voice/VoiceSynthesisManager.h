// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "Sound/SoundWave.h"
#include "VoiceSynthesisManager.generated.h"

/**
 * Voice configuration for the digital human
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FVoiceConfiguration
{
    GENERATED_BODY()

    /** Voice ID for the TTS service */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice")
    FString VoiceID = TEXT("nova-female-1");

    /** Base speaking rate (1.0 = normal) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ClampMin = "0.5", ClampMax = "2.0"))
    float SpeakingRate = 1.0f;

    /** Base pitch (1.0 = normal) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ClampMin = "0.5", ClampMax = "2.0"))
    float Pitch = 1.0f;

    /** Volume level */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Volume = 1.0f;

    /** Enable breathing sounds between sentences */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice")
    bool bEnableBreathing = true;

    /** Natural pause duration between sentences (seconds) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float SentencePauseDuration = 0.3f;

    /** Enable emotion-based voice modulation */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice")
    bool bEmotionModulation = true;
};

/**
 * Lip sync data for a speech segment
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FLipSyncData
{
    GENERATED_BODY()

    /** Total duration of the speech */
    UPROPERTY(BlueprintReadOnly, Category = "LipSync")
    float Duration = 0.0f;

    /** Frame-by-frame lip sync data */
    UPROPERTY(BlueprintReadOnly, Category = "LipSync")
    TArray<FLipSyncFrame> Frames;

    /** Sample rate of the data */
    UPROPERTY(BlueprintReadOnly, Category = "LipSync")
    int32 SampleRate = 60;
};

/**
 * Audio playback state
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FAudioPlaybackState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Playback")
    bool bIsPlaying = false;

    UPROPERTY(BlueprintReadOnly, Category = "Playback")
    float CurrentTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Playback")
    float TotalDuration = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Playback")
    FString CurrentText;

    UPROPERTY(BlueprintReadOnly, Category = "Playback")
    EDigitalHumanEmotion CurrentEmotion = EDigitalHumanEmotion::Neutral;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeechSynthesisComplete, USoundWave*, GeneratedAudio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLipSyncDataReady, const FLipSyncData&, LipSyncData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhonemeChanged, const FLipSyncFrame&, CurrentFrame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpeechPlaybackComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVoiceSynthesisError, const FString&, ErrorMessage);

/**
 * Manages voice synthesis, lip sync generation, and audio playback.
 * Integrates with neural TTS services for natural speech generation.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UVoiceSynthesisManager : public UObject
{
    GENERATED_BODY()

public:
    UVoiceSynthesisManager();

    /** Initialize the voice synthesis manager */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    void Initialize();

    /** Speak the given text with optional emotion */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    void Speak(const FString& Text, EDigitalHumanEmotion Emotion = EDigitalHumanEmotion::Neutral);

    /** Stop current speech */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    void StopSpeaking();

    /** Pause current speech */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    void PauseSpeaking();

    /** Resume paused speech */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    void ResumeSpeaking();

    /** Set voice configuration */
    UFUNCTION(BlueprintCallable, Category = "Voice|Config")
    void SetVoiceConfiguration(const FVoiceConfiguration& Config);

    /** Get current voice configuration */
    UFUNCTION(BlueprintCallable, Category = "Voice|Config")
    FVoiceConfiguration GetVoiceConfiguration() const { return VoiceConfig; }

    /** Set TTS API endpoint */
    UFUNCTION(BlueprintCallable, Category = "Voice|Config")
    void SetTTSEndpoint(const FString& Endpoint, const FString& APIKey);

    /** Check if currently speaking */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    bool IsSpeaking() const;

    /** Get current playback state */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    FAudioPlaybackState GetPlaybackState() const;

    /** Get current lip sync frame */
    UFUNCTION(BlueprintCallable, Category = "Voice|LipSync")
    FLipSyncFrame GetCurrentLipSyncFrame() const;

    /** Get blendshape weights for ARKit compatibility */
    UFUNCTION(BlueprintCallable, Category = "Voice|LipSync")
    TMap<FName, float> GetCurrentBlendshapeWeights() const;

    /** Queue text to be spoken after current speech */
    UFUNCTION(BlueprintCallable, Category = "Voice")
    void QueueSpeech(const FString& Text, EDigitalHumanEmotion Emotion = EDigitalHumanEmotion::Neutral);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Voice|Events")
    FOnSpeechSynthesisComplete OnSynthesisComplete;

    UPROPERTY(BlueprintAssignable, Category = "Voice|Events")
    FOnLipSyncDataReady OnLipSyncDataReady;

    UPROPERTY(BlueprintAssignable, Category = "Voice|Events")
    FOnPhonemeChanged OnPhonemeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Voice|Events")
    FOnSpeechPlaybackComplete OnPlaybackComplete;

    UPROPERTY(BlueprintAssignable, Category = "Voice|Events")
    FOnVoiceSynthesisError OnError;

protected:
    /** Generate speech from text using TTS */
    void GenerateSpeech(const FString& Text, EDigitalHumanEmotion Emotion);

    /** Generate lip sync data from audio */
    FLipSyncData GenerateLipSyncData(const TArray<uint8>& AudioData, int32 SampleRate);

    /** Apply emotion modulation to voice parameters */
    FVoiceConfiguration GetEmotionModulatedConfig(EDigitalHumanEmotion Emotion) const;

    /** Text preprocessing for TTS */
    FString PreprocessText(const FString& Text) const;

    /** Get phoneme from audio analysis */
    EPhoneme AnalyzePhoneme(float AudioAmplitude, float Frequency) const;

    /** Convert phoneme to ARKit blendshape weights */
    TMap<FName, float> PhonemeToBlendshapes(EPhoneme Phoneme, float Intensity) const;

    /** Handle TTS API response */
    void OnTTSResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

    /** Update lip sync during playback */
    void UpdateLipSync(float DeltaTime);

    /** Process speech queue */
    void ProcessSpeechQueue();

private:
    UPROPERTY()
    FVoiceConfiguration VoiceConfig;

    // TTS API configuration
    FString TTSEndpoint;
    FString TTSAPIKey;

    // Current playback state
    FAudioPlaybackState PlaybackState;
    FLipSyncData CurrentLipSyncData;
    int32 CurrentLipSyncFrameIndex = 0;

    // Speech queue
    TArray<TPair<FString, EDigitalHumanEmotion>> SpeechQueue;

    // State
    bool bIsInitialized = false;
    bool bIsPaused = false;

    // Audio component reference (set by character)
    UPROPERTY()
    class UAudioComponent* AudioComponent;

    // Lip sync update timer
    FTimerHandle LipSyncUpdateTimer;

    // ARKit blendshape names
    static const TMap<EPhoneme, TMap<FName, float>> PhonemeBlendshapeMap;
};
