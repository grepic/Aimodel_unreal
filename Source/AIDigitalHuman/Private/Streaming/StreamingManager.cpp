// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Streaming/StreamingManager.h"
#include "AIDigitalHumanModule.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/AudioComponent.h"

UStreamingManager::UStreamingManager()
{
    Configuration.Quality = EStreamingQuality::Quality_4K;
    Configuration.TargetFPS = 60;
    Configuration.VideoBitrate = 20000;
    Configuration.AudioBitrate = 320;
    Configuration.bUseHardwareEncoder = true;
    Configuration.bLowLatencyMode = true;
}

void UStreamingManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing Streaming Manager..."));

    bIsInitialized = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Streaming Manager Initialized"));
}

bool UStreamingManager::StartStreaming()
{
    if (bIsStreaming)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Already streaming"));
        return false;
    }

    if (Configuration.RTMPUrl.IsEmpty())
    {
        OnStreamingError.Broadcast(TEXT("RTMP URL not configured"));
        return false;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Starting stream to: %s"), *Configuration.RTMPUrl);

    // Initialize encoder
    if (!InitializeEncoder())
    {
        OnStreamingError.Broadcast(TEXT("Failed to initialize encoder"));
        return false;
    }

    // Setup RTMP connection
    if (!SetupRTMPConnection())
    {
        OnStreamingError.Broadcast(TEXT("Failed to connect to RTMP server"));
        return false;
    }

    // Reset stats
    Stats = FStreamingStats();
    Stats.bIsStreaming = true;
    StreamStartTime = FDateTime::Now();

    // Get resolution string
    FIntPoint Resolution = GetResolutionFromQuality(Configuration.Quality);
    Stats.CurrentResolution = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
    Stats.CurrentFPS = Configuration.TargetFPS;

    // Start capture timer
    if (UWorld* World = GetWorld())
    {
        float CaptureInterval = 1.0f / Configuration.TargetFPS;
        World->GetTimerManager().SetTimer(
            CaptureTimer,
            this,
            &UStreamingManager::CaptureFrame,
            CaptureInterval,
            true
        );

        // Stats update every second
        World->GetTimerManager().SetTimer(
            StatsTimer,
            this,
            &UStreamingManager::UpdateStats,
            1.0f,
            true
        );
    }

    bIsStreaming = true;
    OnStreamingStarted.Broadcast();

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Streaming started: %s @ %d FPS"), *Stats.CurrentResolution, Stats.CurrentFPS);

    return true;
}

void UStreamingManager::StopStreaming()
{
    if (!bIsStreaming)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Stopping stream..."));

    // Clear timers
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(CaptureTimer);
        World->GetTimerManager().ClearTimer(StatsTimer);
    }

    bIsStreaming = false;
    Stats.bIsStreaming = false;

    OnStreamingStopped.Broadcast(TEXT("Manual stop"));

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Streaming stopped. Total uptime: %.0f seconds, Frames sent: %lld"),
        Stats.UptimeSeconds, Stats.TotalFramesSent);
}

void UStreamingManager::SetConfiguration(const FStreamingConfiguration& Config)
{
    bool bWasStreaming = bIsStreaming;

    if (bWasStreaming)
    {
        StopStreaming();
    }

    Configuration = Config;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Streaming configuration updated"));

    if (bWasStreaming)
    {
        StartStreaming();
    }
}

void UStreamingManager::SetCaptureSource(USceneCaptureComponent2D* InCaptureComponent)
{
    CaptureComponent = InCaptureComponent;
}

void UStreamingManager::SetAudioSource(UAudioComponent* InAudioSource)
{
    AudioSource = InAudioSource;
}

void UStreamingManager::SetAdaptiveBitrate(bool bEnabled)
{
    bAdaptiveBitrate = bEnabled;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Adaptive bitrate: %s"), bEnabled ? TEXT("Enabled") : TEXT("Disabled"));
}

void UStreamingManager::AdjustQuality(EStreamingQuality NewQuality)
{
    if (Configuration.Quality == NewQuality)
    {
        return;
    }

    Configuration.Quality = NewQuality;

    FIntPoint Resolution = GetResolutionFromQuality(NewQuality);
    Stats.CurrentResolution = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Quality adjusted to: %s"), *Stats.CurrentResolution);

    OnStreamingWarning.Broadcast(FString::Printf(TEXT("Quality changed to %s"), *Stats.CurrentResolution));
}

bool UStreamingManager::InitializeEncoder()
{
    // In a real implementation, this would initialize the video encoder
    // (NVENC, AMF, or software encoder)

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing encoder: %s, Hardware: %s"),
        *Configuration.EncoderPreset,
        Configuration.bUseHardwareEncoder ? TEXT("Yes") : TEXT("No"));

    return true;
}

bool UStreamingManager::SetupRTMPConnection()
{
    // In a real implementation, this would establish the RTMP connection

    FString FullUrl = Configuration.RTMPUrl;
    if (!Configuration.StreamKey.IsEmpty())
    {
        FullUrl += TEXT("/") + Configuration.StreamKey;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Connecting to RTMP: %s"), *Configuration.RTMPUrl);

    return true;
}

void UStreamingManager::CaptureFrame()
{
    if (!bIsStreaming)
    {
        return;
    }

    // In a real implementation, this would:
    // 1. Capture the render target from SceneCaptureComponent2D
    // 2. Encode the frame using hardware/software encoder
    // 3. Send the encoded frame over RTMP
    // 4. Mix in audio from AudioComponent

    Stats.TotalFramesSent++;

    // Simulate encoding latency
    Stats.EncoderLatencyMs = FMath::RandRange(1.0f, 3.0f);

    // Check performance
    CheckPerformance();

    // Handle adaptive bitrate if enabled
    if (bAdaptiveBitrate)
    {
        HandleAdaptiveBitrate();
    }
}

void UStreamingManager::UpdateStats()
{
    if (!bIsStreaming)
    {
        return;
    }

    Stats.UptimeSeconds = (FDateTime::Now() - StreamStartTime).GetTotalSeconds();

    // Calculate current bitrate from frames sent
    static int64 LastBytesSent = 0;
    float BytesSentThisSecond = Stats.TotalBytesSent - LastBytesSent;
    LastBytesSent = Stats.TotalBytesSent;

    Stats.CurrentBitrate = BytesSentThisSecond * 8 / 1000.0f; // kbps
    Stats.AverageBitrate = (Stats.TotalBytesSent * 8 / 1000.0f) / FMath::Max(1.0f, Stats.UptimeSeconds);

    // Calculate frame drop percentage
    float ExpectedFrames = Stats.UptimeSeconds * Configuration.TargetFPS;
    if (ExpectedFrames > 0)
    {
        Stats.FrameDropPercentage = (1.0f - (Stats.TotalFramesSent / ExpectedFrames)) * 100.0f;
    }

    OnStatsUpdated.Broadcast(Stats);
}

void UStreamingManager::CheckPerformance()
{
    // Monitor frame time
    float CurrentFrameTime = FApp::GetDeltaTime() * 1000.0f;
    float TargetFrameTime = 1000.0f / Configuration.TargetFPS;

    // Smooth average
    AverageFrameTime = AverageFrameTime * 0.9f + CurrentFrameTime * 0.1f;

    if (AverageFrameTime > TargetFrameTime * 1.2f)
    {
        ConsecutiveLowFrames++;

        if (ConsecutiveLowFrames >= 60) // ~1 second of low FPS
        {
            OnStreamingWarning.Broadcast(
                FString::Printf(TEXT("Performance warning: Frame time %.1fms (target: %.1fms)"),
                    AverageFrameTime, TargetFrameTime)
            );
            ConsecutiveLowFrames = 0;
        }
    }
    else
    {
        ConsecutiveLowFrames = 0;
    }
}

void UStreamingManager::HandleAdaptiveBitrate()
{
    // If consistently dropping frames, reduce quality
    if (Stats.FrameDropPercentage > 5.0f && Configuration.Quality != EStreamingQuality::Quality_720p)
    {
        switch (Configuration.Quality)
        {
        case EStreamingQuality::Quality_4K:
            AdjustQuality(EStreamingQuality::Quality_1440p);
            break;
        case EStreamingQuality::Quality_1440p:
            AdjustQuality(EStreamingQuality::Quality_1080p);
            break;
        case EStreamingQuality::Quality_1080p:
            AdjustQuality(EStreamingQuality::Quality_720p);
            break;
        default:
            break;
        }
    }
    // If running smoothly and below target quality, consider upgrading
    else if (Stats.FrameDropPercentage < 1.0f && AverageFrameTime < (1000.0f / Configuration.TargetFPS) * 0.8f)
    {
        switch (Configuration.Quality)
        {
        case EStreamingQuality::Quality_720p:
            AdjustQuality(EStreamingQuality::Quality_1080p);
            break;
        case EStreamingQuality::Quality_1080p:
            AdjustQuality(EStreamingQuality::Quality_1440p);
            break;
        case EStreamingQuality::Quality_1440p:
            AdjustQuality(EStreamingQuality::Quality_4K);
            break;
        default:
            break;
        }
    }
}

FIntPoint UStreamingManager::GetResolutionFromQuality(EStreamingQuality Quality) const
{
    switch (Quality)
    {
    case EStreamingQuality::Quality_720p:
        return FIntPoint(1280, 720);
    case EStreamingQuality::Quality_1080p:
        return FIntPoint(1920, 1080);
    case EStreamingQuality::Quality_1440p:
        return FIntPoint(2560, 1440);
    case EStreamingQuality::Quality_4K:
    default:
        return FIntPoint(3840, 2160);
    }
}
