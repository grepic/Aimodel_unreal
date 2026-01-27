// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIDigitalHumanTypes.generated.h"

/**
 * Emotion states for the digital human
 */
UENUM(BlueprintType)
enum class EDigitalHumanEmotion : uint8
{
    Neutral     UMETA(DisplayName = "Neutral"),
    Happy       UMETA(DisplayName = "Happy"),
    Sad         UMETA(DisplayName = "Sad"),
    Surprised   UMETA(DisplayName = "Surprised"),
    Angry       UMETA(DisplayName = "Angry"),
    Fearful     UMETA(DisplayName = "Fearful"),
    Disgusted   UMETA(DisplayName = "Disgusted"),
    Contempt    UMETA(DisplayName = "Contempt"),
    Flirty      UMETA(DisplayName = "Flirty"),
    Excited     UMETA(DisplayName = "Excited"),
    Thinking    UMETA(DisplayName = "Thinking"),
    Confused    UMETA(DisplayName = "Confused")
};

/**
 * Animation layer types for modular animation system
 */
UENUM(BlueprintType)
enum class EAnimationLayer : uint8
{
    Base        UMETA(DisplayName = "Base Layer"),
    Idle        UMETA(DisplayName = "Idle Layer"),
    Movement    UMETA(DisplayName = "Movement Layer"),
    Interaction UMETA(DisplayName = "Interaction Layer"),
    Facial      UMETA(DisplayName = "Facial Layer"),
    Additive    UMETA(DisplayName = "Additive Layer")
};

/**
 * User tier levels for monetization priority
 */
UENUM(BlueprintType)
enum class EUserTier : uint8
{
    Free        UMETA(DisplayName = "Free User"),
    Subscriber  UMETA(DisplayName = "Subscriber"),
    Premium     UMETA(DisplayName = "Premium"),
    VIP         UMETA(DisplayName = "VIP"),
    Whale       UMETA(DisplayName = "Whale (Top Spender)")
};

/**
 * Chat message priority levels
 */
UENUM(BlueprintType)
enum class EChatPriority : uint8
{
    Low         UMETA(DisplayName = "Low Priority"),
    Normal      UMETA(DisplayName = "Normal Priority"),
    High        UMETA(DisplayName = "High Priority"),
    Urgent      UMETA(DisplayName = "Urgent Priority"),
    Critical    UMETA(DisplayName = "Critical Priority")
};

/**
 * Streaming quality presets
 */
UENUM(BlueprintType)
enum class EStreamingQuality : uint8
{
    Quality_720p    UMETA(DisplayName = "720p (1280x720)"),
    Quality_1080p   UMETA(DisplayName = "1080p (1920x1080)"),
    Quality_1440p   UMETA(DisplayName = "1440p (2560x1440)"),
    Quality_4K      UMETA(DisplayName = "4K (3840x2160)")
};

/**
 * Lip sync phoneme types (ARKit compatible)
 */
UENUM(BlueprintType)
enum class EPhoneme : uint8
{
    Silence     UMETA(DisplayName = "Silence"),
    AA          UMETA(DisplayName = "AA (as in 'bat')"),
    AE          UMETA(DisplayName = "AE (as in 'say')"),
    AH          UMETA(DisplayName = "AH (as in 'father')"),
    AO          UMETA(DisplayName = "AO (as in 'dog')"),
    AW          UMETA(DisplayName = "AW (as in 'how')"),
    AY          UMETA(DisplayName = "AY (as in 'hide')"),
    B           UMETA(DisplayName = "B"),
    CH          UMETA(DisplayName = "CH"),
    D           UMETA(DisplayName = "D"),
    DH          UMETA(DisplayName = "DH (as in 'the')"),
    EH          UMETA(DisplayName = "EH (as in 'red')"),
    ER          UMETA(DisplayName = "ER (as in 'bird')"),
    EY          UMETA(DisplayName = "EY (as in 'say')"),
    F           UMETA(DisplayName = "F"),
    G           UMETA(DisplayName = "G"),
    HH          UMETA(DisplayName = "HH"),
    IH          UMETA(DisplayName = "IH (as in 'bit')"),
    IY          UMETA(DisplayName = "IY (as in 'beat')"),
    JH          UMETA(DisplayName = "JH"),
    K           UMETA(DisplayName = "K"),
    L           UMETA(DisplayName = "L"),
    M           UMETA(DisplayName = "M"),
    N           UMETA(DisplayName = "N"),
    NG          UMETA(DisplayName = "NG"),
    OW          UMETA(DisplayName = "OW (as in 'go')"),
    OY          UMETA(DisplayName = "OY (as in 'boy')"),
    P           UMETA(DisplayName = "P"),
    R           UMETA(DisplayName = "R"),
    S           UMETA(DisplayName = "S"),
    SH          UMETA(DisplayName = "SH"),
    T           UMETA(DisplayName = "T"),
    TH          UMETA(DisplayName = "TH (as in 'thin')"),
    UH          UMETA(DisplayName = "UH (as in 'book')"),
    UW          UMETA(DisplayName = "UW (as in 'boot')"),
    V           UMETA(DisplayName = "V"),
    W           UMETA(DisplayName = "W"),
    Y           UMETA(DisplayName = "Y"),
    Z           UMETA(DisplayName = "Z"),
    ZH          UMETA(DisplayName = "ZH (as in 'measure')")
};

/**
 * Camera shot types for the cinematic camera system
 */
UENUM(BlueprintType)
enum class ECameraShotType : uint8
{
    CloseUp         UMETA(DisplayName = "Close Up"),
    MediumCloseUp   UMETA(DisplayName = "Medium Close Up"),
    MediumShot      UMETA(DisplayName = "Medium Shot"),
    MediumWideShot  UMETA(DisplayName = "Medium Wide Shot"),
    WideShot        UMETA(DisplayName = "Wide Shot"),
    OverTheShoulder UMETA(DisplayName = "Over The Shoulder"),
    Dutch           UMETA(DisplayName = "Dutch Angle"),
    LowAngle        UMETA(DisplayName = "Low Angle"),
    HighAngle       UMETA(DisplayName = "High Angle"),
    POV             UMETA(DisplayName = "Point of View")
};

/**
 * Chat message data structure
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FChatMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    FString Username;

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    FString Message;

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    EUserTier UserTier = EUserTier::Free;

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    EChatPriority Priority = EChatPriority::Normal;

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    bool bIsPaidMessage = false;

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    float TipAmount = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Chat")
    FString UserID;

    FChatMessage()
        : Timestamp(FDateTime::Now())
    {}
};

/**
 * Monetization event data
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FMonetizationEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    FString EventType;

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    FString UserID;

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    FString Username;

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    float Amount = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    EUserTier NewTier = EUserTier::Free;

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    FString Message;

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadWrite, Category = "Monetization")
    TMap<FString, FString> Metadata;

    FMonetizationEvent()
        : Timestamp(FDateTime::Now())
    {}
};

/**
 * AI Response data from LLM
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FAIResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    FString ResponseText;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    EDigitalHumanEmotion Emotion = EDigitalHumanEmotion::Neutral;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    float EmotionIntensity = 0.5f;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    TArray<FString> AnimationTriggers;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    ECameraShotType SuggestedCameraShot = ECameraShotType::MediumCloseUp;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    bool bShouldChangePose = false;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    FString TargetPose;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    bool bIsThinking = false;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    float SpeechRate = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "AI")
    float SpeechPitch = 1.0f;
};

/**
 * Lip sync frame data
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FLipSyncFrame
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "LipSync")
    float Timestamp = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "LipSync")
    EPhoneme Phoneme = EPhoneme::Silence;

    UPROPERTY(BlueprintReadWrite, Category = "LipSync")
    float Intensity = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "LipSync")
    TMap<FName, float> BlendshapeWeights;
};

/**
 * Character synthesis verification flags
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FCharacterVerificationFlags
{
    GENERATED_BODY()

    /** Character is fully synthetic, not based on any real person */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    bool bIsFullySynthetic = true;

    /** Character is verified as 18+ in appearance and behavior */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    bool bAgeVerified18Plus = true;

    /** No face cloning was used */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    bool bNoFaceCloning = true;

    /** No likeness of existing persons */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    bool bNoRealLikeness = true;

    /** Procedural generation seed for reproducibility */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    int32 GenerationSeed = 0;

    /** Generation timestamp */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    FDateTime GenerationTimestamp;

    /** Verification hash */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    FString VerificationHash;

    FCharacterVerificationFlags()
        : GenerationTimestamp(FDateTime::Now())
    {}

    /** Validate all flags are properly set */
    bool IsValid() const
    {
        return bIsFullySynthetic && bAgeVerified18Plus && bNoFaceCloning && bNoRealLikeness;
    }
};

/**
 * Performance metrics for monitoring
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float CurrentFPS = 60.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float AverageFPS = 60.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float FrameTimeMs = 16.67f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float GPUTimeMs = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float CPUTimeMs = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int64 VideoMemoryUsedMB = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int64 SystemMemoryUsedMB = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    EStreamingQuality CurrentQuality = EStreamingQuality::Quality_4K;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    bool bDLSSEnabled = false;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    bool bTSREnabled = false;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    bool bLumenEnabled = true;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    bool bNaniteEnabled = true;
};

// Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageReceived, const FChatMessage&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonetizationEvent, const FMonetizationEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIResponseReady, const FAIResponse&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmotionChanged, EDigitalHumanEmotion, NewEmotion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeechStarted, const FString&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpeechEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnimationTriggered, const FString&, AnimationName, float, BlendTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraShotChanged, ECameraShotType, NewShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceWarning, const FString&, WarningMessage);
