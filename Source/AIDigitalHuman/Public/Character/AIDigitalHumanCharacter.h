// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/AIDigitalHumanTypes.h"
#include "AIDigitalHumanCharacter.generated.h"

class UGroomComponent;
class USkeletalMeshComponent;
class UAudioComponent;
class UAIDigitalHumanAnimInstance;

/**
 * Eye state for realistic eye behavior
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FEyeState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Eyes")
    FVector2D GazeDirection = FVector2D::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = "Eyes")
    float PupilDilation = 0.5f;

    UPROPERTY(BlueprintReadWrite, Category = "Eyes")
    float BlinkTimer = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Eyes")
    bool bIsBlinking = false;

    UPROPERTY(BlueprintReadWrite, Category = "Eyes")
    float WetnessAmount = 0.5f;

    UPROPERTY(BlueprintReadWrite, Category = "Eyes")
    float SaccadeTimer = 0.0f;
};

/**
 * Base character class for the AI Digital Human.
 * Extends ACharacter with MetaHuman-specific features.
 */
UCLASS(Blueprintable)
class AIDIGITALHUMAN_API AAIDigitalHumanCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAIDigitalHumanCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ==================== Emotion System ====================

    /** Set the current emotion */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Emotion")
    void SetEmotion(EDigitalHumanEmotion NewEmotion, float Intensity = 0.5f, float BlendTime = 0.3f);

    /** Get current emotion */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Emotion")
    EDigitalHumanEmotion GetCurrentEmotion() const { return CurrentEmotion; }

    /** Get current emotion intensity */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Emotion")
    float GetEmotionIntensity() const { return EmotionIntensity; }

    // ==================== Lip Sync ====================

    /** Apply lip sync frame */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|LipSync")
    void ApplyLipSyncFrame(const FLipSyncFrame& Frame);

    /** Set blendshape weights directly */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|LipSync")
    void SetBlendshapeWeights(const TMap<FName, float>& Weights);

    /** Clear all blendshape weights */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|LipSync")
    void ClearBlendshapeWeights();

    // ==================== Animation ====================

    /** Play an animation montage */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Animation")
    void PlayAnimationMontage(UAnimMontage* Montage, float PlayRate = 1.0f);

    /** Trigger animation by name */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Animation")
    void TriggerAnimation(const FString& AnimationName, float BlendTime = 0.25f);

    /** Set idle variation */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Animation")
    void SetIdleVariation(int32 VariationIndex);

    // ==================== Eyes ====================

    /** Set gaze target location */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Eyes")
    void SetGazeTarget(FVector WorldLocation);

    /** Set gaze target actor */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Eyes")
    void SetGazeTargetActor(AActor* Target);

    /** Clear gaze target */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Eyes")
    void ClearGazeTarget();

    /** Trigger a blink */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Eyes")
    void TriggerBlink();

    /** Set pupil dilation */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Eyes")
    void SetPupilDilation(float Dilation);

    // ==================== Voice ====================

    /** Get audio component */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Voice")
    UAudioComponent* GetVoiceAudioComponent() const { return VoiceAudioComponent; }

    // ==================== Verification ====================

    /** Get character verification flags */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Verification")
    FCharacterVerificationFlags GetVerificationFlags() const { return VerificationFlags; }

    /** Verify character is synthetic */
    UFUNCTION(BlueprintCallable, Category = "Digital Human|Verification")
    bool VerifyIsSynthetic() const { return VerificationFlags.IsValid(); }

protected:
    // ==================== Components ====================

    /** Face mesh component (MetaHuman face) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* FaceMeshComponent;

    /** Body mesh component (if separate from main skeletal mesh) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* BodyMeshComponent;

    /** Hair groom component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UGroomComponent* HairGroomComponent;

    /** Eyebrow groom component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UGroomComponent* EyebrowGroomComponent;

    /** Eyelash groom component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UGroomComponent* EyelashGroomComponent;

    /** Voice audio component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAudioComponent* VoiceAudioComponent;

    // ==================== Emotion Configuration ====================

    /** Current emotion */
    UPROPERTY(BlueprintReadOnly, Category = "Emotion")
    EDigitalHumanEmotion CurrentEmotion = EDigitalHumanEmotion::Neutral;

    /** Current emotion intensity */
    UPROPERTY(BlueprintReadOnly, Category = "Emotion")
    float EmotionIntensity = 0.0f;

    /** Target emotion for blending */
    UPROPERTY(BlueprintReadOnly, Category = "Emotion")
    EDigitalHumanEmotion TargetEmotion = EDigitalHumanEmotion::Neutral;

    /** Target emotion intensity */
    UPROPERTY(BlueprintReadOnly, Category = "Emotion")
    float TargetEmotionIntensity = 0.0f;

    /** Emotion blend time */
    UPROPERTY(BlueprintReadOnly, Category = "Emotion")
    float EmotionBlendTime = 0.3f;

    /** Current emotion blend progress */
    UPROPERTY(BlueprintReadOnly, Category = "Emotion")
    float EmotionBlendProgress = 1.0f;

    // ==================== Eye Configuration ====================

    /** Current eye state */
    UPROPERTY(BlueprintReadOnly, Category = "Eyes")
    FEyeState EyeState;

    /** Gaze target location */
    UPROPERTY(BlueprintReadOnly, Category = "Eyes")
    FVector GazeTargetLocation;

    /** Gaze target actor */
    UPROPERTY(BlueprintReadOnly, Category = "Eyes")
    AActor* GazeTargetActor;

    /** Average blink interval */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
    float AverageBlinkInterval = 4.0f;

    /** Blink interval variance */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
    float BlinkIntervalVariance = 2.0f;

    /** Micro saccade frequency */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
    float SaccadeFrequency = 0.3f;

    /** Micro saccade amplitude */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
    float SaccadeAmplitude = 2.0f;

    // ==================== Verification ====================

    /** Character verification flags */
    UPROPERTY(BlueprintReadOnly, Category = "Verification")
    FCharacterVerificationFlags VerificationFlags;

    // ==================== Animation Mappings ====================

    /** Map of animation names to montages */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TMap<FString, UAnimMontage*> AnimationMontages;

    // ==================== Internal Methods ====================

    /** Update emotion blending */
    virtual void UpdateEmotionBlend(float DeltaTime);

    /** Update eye behavior */
    virtual void UpdateEyeBehavior(float DeltaTime);

    /** Update gaze direction */
    virtual void UpdateGaze(float DeltaTime);

    /** Process micro saccades */
    virtual void ProcessMicroSaccades(float DeltaTime);

    /** Process automatic blinking */
    virtual void ProcessBlinking(float DeltaTime);

    /** Apply emotion to face */
    virtual void ApplyEmotionToFace(EDigitalHumanEmotion Emotion, float Intensity);

    /** Get blendshape weights for emotion */
    TMap<FName, float> GetEmotionBlendshapes(EDigitalHumanEmotion Emotion, float Intensity) const;

private:
    /** Timer for next blink */
    float NextBlinkTime = 0.0f;

    /** Current lip sync weights */
    TMap<FName, float> CurrentLipSyncWeights;

    /** Current emotion weights */
    TMap<FName, float> CurrentEmotionWeights;
};
