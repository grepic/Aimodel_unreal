// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "StreamingManager.generated.h"

/**
 * Streaming configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FStreamingConfiguration
{
    GENERATED_BODY()

    /** Output resolution */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    EStreamingQuality Quality = EStreamingQuality::Quality_4K;

    /** Target framerate */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    int32 TargetFPS = 60;

    /** Video bitrate (kbps) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    int32 VideoBitrate = 20000;

    /** Audio bitrate (kbps) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    int32 AudioBitrate = 320;

    /** Encoder preset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    FString EncoderPreset = TEXT("Quality");

    /** Use hardware encoding (NVENC/AMF) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    bool bUseHardwareEncoder = true;

    /** Enable low latency mode */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    bool bLowLatencyMode = true;

    /** RTMP server URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    FString RTMPUrl;

    /** Stream key */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    FString StreamKey;

    /** Audio sample rate */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    int32 AudioSampleRate = 48000;

    /** Audio channels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    int32 AudioChannels = 2;
};

/**
 * Streaming statistics
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FStreamingStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    bool bIsStreaming = false;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float UptimeSeconds = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int64 TotalBytesSent = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float CurrentBitrate = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float AverageBitrate = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 DroppedFrames = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float FrameDropPercentage = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int64 TotalFramesSent = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float EncoderLatencyMs = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float NetworkLatencyMs = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    FString CurrentResolution;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 CurrentFPS = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStreamingStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStreamingStopped, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStreamingError, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStreamingWarning, const FString&, WarningMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStreamingStatsUpdated, const FStreamingStats&, Stats);

/**
 * Manages the streaming pipeline for output to external platforms.
 * Handles video/audio capture, encoding, and RTMP output.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UStreamingManager : public UObject
{
    GENERATED_BODY()

public:
    UStreamingManager();

    /** Initialize the streaming manager */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void Initialize();

    /** Start streaming */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    bool StartStreaming();

    /** Stop streaming */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void StopStreaming();

    /** Check if currently streaming */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    bool IsStreaming() const { return bIsStreaming; }

    /** Set streaming configuration */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void SetConfiguration(const FStreamingConfiguration& Config);

    /** Get current configuration */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    FStreamingConfiguration GetConfiguration() const { return Configuration; }

    /** Get current streaming statistics */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    FStreamingStats GetStats() const { return Stats; }

    /** Set the capture source (camera/scene capture) */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void SetCaptureSource(class USceneCaptureComponent2D* CaptureComponent);

    /** Set audio source */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void SetAudioSource(class UAudioComponent* AudioSource);

    /** Enable/disable adaptive bitrate */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void SetAdaptiveBitrate(bool bEnabled);

    /** Manually adjust quality for performance */
    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void AdjustQuality(EStreamingQuality NewQuality);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Streaming|Events")
    FOnStreamingStarted OnStreamingStarted;

    UPROPERTY(BlueprintAssignable, Category = "Streaming|Events")
    FOnStreamingStopped OnStreamingStopped;

    UPROPERTY(BlueprintAssignable, Category = "Streaming|Events")
    FOnStreamingError OnStreamingError;

    UPROPERTY(BlueprintAssignable, Category = "Streaming|Events")
    FOnStreamingWarning OnStreamingWarning;

    UPROPERTY(BlueprintAssignable, Category = "Streaming|Events")
    FOnStreamingStatsUpdated OnStatsUpdated;

protected:
    /** Initialize video encoder */
    bool InitializeEncoder();

    /** Setup RTMP connection */
    bool SetupRTMPConnection();

    /** Capture and encode frame */
    void CaptureFrame();

    /** Update statistics */
    void UpdateStats();

    /** Check for performance issues */
    void CheckPerformance();

    /** Handle adaptive bitrate adjustments */
    void HandleAdaptiveBitrate();

    /** Get resolution from quality setting */
    FIntPoint GetResolutionFromQuality(EStreamingQuality Quality) const;

private:
    FStreamingConfiguration Configuration;
    FStreamingStats Stats;

    // Capture sources
    UPROPERTY()
    class USceneCaptureComponent2D* CaptureComponent;

    UPROPERTY()
    class UAudioComponent* AudioSource;

    // State
    bool bIsInitialized = false;
    bool bIsStreaming = false;
    bool bAdaptiveBitrate = true;

    // Timing
    FDateTime StreamStartTime;
    FTimerHandle CaptureTimer;
    FTimerHandle StatsTimer;

    // Performance monitoring
    int32 ConsecutiveLowFrames = 0;
    float AverageFrameTime = 0.0f;
};
