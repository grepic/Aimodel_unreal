// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Core/AIDigitalHumanTypes.h"
#include "AIDigitalHumanAnimInstance.generated.h"

/**
 * Animation state for the digital human
 */
UENUM(BlueprintType)
enum class EDigitalHumanAnimState : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    Talking         UMETA(DisplayName = "Talking"),
    Reacting        UMETA(DisplayName = "Reacting"),
    Gesturing       UMETA(DisplayName = "Gesturing"),
    Dancing         UMETA(DisplayName = "Dancing"),
    Transitioning   UMETA(DisplayName = "Transitioning")
};

/**
 * Animation layer blend weights
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FAnimationLayerWeights
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float BaseLayerWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float IdleLayerWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float GestureLayerWeight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FacialLayerWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float AdditiveLayerWeight = 0.0f;
};

/**
 * Breathing parameters
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FBreathingParameters
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float BreathingRate = 15.0f; // Breaths per minute

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float BreathingDepth = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    bool bIsBreathingHeavy = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float ChestExpansion = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float ShoulderRise = 0.0f;
};

/**
 * Animation instance for the AI Digital Human.
 * Manages animation state machine, layer blending, and procedural animation.
 */
UCLASS()
class AIDIGITALHUMAN_API UAIDigitalHumanAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UAIDigitalHumanAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // ==================== State Management ====================

    /** Get current animation state */
    UFUNCTION(BlueprintCallable, Category = "Animation|State")
    EDigitalHumanAnimState GetCurrentState() const { return CurrentState; }

    /** Set animation state */
    UFUNCTION(BlueprintCallable, Category = "Animation|State")
    void SetAnimationState(EDigitalHumanAnimState NewState);

    /** Get current emotion */
    UFUNCTION(BlueprintCallable, Category = "Animation|State")
    EDigitalHumanEmotion GetCurrentEmotion() const { return CurrentEmotion; }

    /** Set current emotion */
    UFUNCTION(BlueprintCallable, Category = "Animation|State")
    void SetEmotion(EDigitalHumanEmotion NewEmotion, float Intensity);

    // ==================== Layer Control ====================

    /** Get layer weights */
    UFUNCTION(BlueprintCallable, Category = "Animation|Layers")
    FAnimationLayerWeights GetLayerWeights() const { return LayerWeights; }

    /** Set layer weights */
    UFUNCTION(BlueprintCallable, Category = "Animation|Layers")
    void SetLayerWeights(const FAnimationLayerWeights& Weights);

    /** Set specific layer weight */
    UFUNCTION(BlueprintCallable, Category = "Animation|Layers")
    void SetLayerWeight(EAnimationLayer Layer, float Weight);

    // ==================== Idle Animation ====================

    /** Set idle variation index */
    UFUNCTION(BlueprintCallable, Category = "Animation|Idle")
    void SetIdleVariation(int32 VariationIndex);

    /** Get current idle variation */
    UFUNCTION(BlueprintCallable, Category = "Animation|Idle")
    int32 GetIdleVariation() const { return IdleVariationIndex; }

    /** Set idle weight shifting amount */
    UFUNCTION(BlueprintCallable, Category = "Animation|Idle")
    void SetIdleWeightShift(float Amount);

    // ==================== Talking Animation ====================

    /** Set talking state */
    UFUNCTION(BlueprintCallable, Category = "Animation|Talking")
    void SetIsTalking(bool bTalking);

    /** Get if currently talking */
    UFUNCTION(BlueprintCallable, Category = "Animation|Talking")
    bool IsTalking() const { return bIsTalking; }

    /** Set speech intensity (0-1) */
    UFUNCTION(BlueprintCallable, Category = "Animation|Talking")
    void SetSpeechIntensity(float Intensity);

    // ==================== Gesture Control ====================

    /** Trigger gesture animation */
    UFUNCTION(BlueprintCallable, Category = "Animation|Gestures")
    void TriggerGesture(const FString& GestureName);

    /** Set gesture blend weight */
    UFUNCTION(BlueprintCallable, Category = "Animation|Gestures")
    void SetGestureWeight(float Weight);

    // ==================== Breathing ====================

    /** Get breathing parameters */
    UFUNCTION(BlueprintCallable, Category = "Animation|Breathing")
    FBreathingParameters GetBreathingParameters() const { return BreathingParams; }

    /** Set breathing parameters */
    UFUNCTION(BlueprintCallable, Category = "Animation|Breathing")
    void SetBreathingParameters(const FBreathingParameters& Params);

    /** Set breathing rate multiplier */
    UFUNCTION(BlueprintCallable, Category = "Animation|Breathing")
    void SetBreathingRateMultiplier(float Multiplier);

    // ==================== Look At ====================

    /** Set look at target */
    UFUNCTION(BlueprintCallable, Category = "Animation|LookAt")
    void SetLookAtTarget(FVector WorldLocation);

    /** Set look at weight */
    UFUNCTION(BlueprintCallable, Category = "Animation|LookAt")
    void SetLookAtWeight(float Weight);

protected:
    // ==================== State Variables ====================

    /** Current animation state */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|State")
    EDigitalHumanAnimState CurrentState = EDigitalHumanAnimState::Idle;

    /** Previous animation state */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|State")
    EDigitalHumanAnimState PreviousState = EDigitalHumanAnimState::Idle;

    /** Current emotion */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|State")
    EDigitalHumanEmotion CurrentEmotion = EDigitalHumanEmotion::Neutral;

    /** Emotion intensity */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|State")
    float EmotionIntensity = 0.0f;

    // ==================== Layer Weights ====================

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Layers")
    FAnimationLayerWeights LayerWeights;

    // ==================== Idle State ====================

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Idle")
    int32 IdleVariationIndex = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Idle")
    float IdleTimer = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Idle")
    float IdleWeightShift = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Idle")
    float IdleVariationChangeInterval = 30.0f;

    // ==================== Talking State ====================

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Talking")
    bool bIsTalking = false;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Talking")
    float SpeechIntensity = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Talking")
    float TalkingGestureWeight = 0.0f;

    // ==================== Breathing ====================

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Breathing")
    FBreathingParameters BreathingParams;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Breathing")
    float BreathingTimer = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|Breathing")
    float BreathingPhase = 0.0f;

    // ==================== Look At ====================

    UPROPERTY(BlueprintReadOnly, Category = "Animation|LookAt")
    FVector LookAtTarget = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|LookAt")
    float LookAtWeight = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Animation|LookAt")
    FRotator CurrentHeadRotation = FRotator::ZeroRotator;

    // ==================== Internal Methods ====================

    /** Update idle animation logic */
    void UpdateIdleAnimation(float DeltaTime);

    /** Update talking animation logic */
    void UpdateTalkingAnimation(float DeltaTime);

    /** Update breathing procedural animation */
    void UpdateBreathing(float DeltaTime);

    /** Update look at logic */
    void UpdateLookAt(float DeltaTime);

    /** Get gesture intensity based on emotion */
    float GetEmotionGestureIntensity() const;

private:
    /** Owner character reference */
    UPROPERTY()
    class AAIDigitalHumanCharacter* OwnerCharacter;

    /** Timer for idle variation changes */
    float IdleVariationTimer = 0.0f;

    /** Number of available idle variations */
    int32 NumIdleVariations = 4;
};
