// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Animation/AIDigitalHumanAnimInstance.h"
#include "Character/AIDigitalHumanCharacter.h"
#include "AIDigitalHumanModule.h"

UAIDigitalHumanAnimInstance::UAIDigitalHumanAnimInstance()
{
    // Default breathing parameters
    BreathingParams.BreathingRate = 15.0f;
    BreathingParams.BreathingDepth = 1.0f;

    // Default layer weights
    LayerWeights.BaseLayerWeight = 1.0f;
    LayerWeights.IdleLayerWeight = 1.0f;
    LayerWeights.FacialLayerWeight = 1.0f;
}

void UAIDigitalHumanAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Get owner character
    OwnerCharacter = Cast<AAIDigitalHumanCharacter>(TryGetPawnOwner());

    if (OwnerCharacter)
    {
        UE_LOG(LogAIDigitalHuman, Log, TEXT("Animation instance initialized for %s"), *OwnerCharacter->GetName());
    }
}

void UAIDigitalHumanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwnerCharacter)
    {
        OwnerCharacter = Cast<AAIDigitalHumanCharacter>(TryGetPawnOwner());
        if (!OwnerCharacter)
        {
            return;
        }
    }

    // Update subsystems
    UpdateIdleAnimation(DeltaSeconds);
    UpdateTalkingAnimation(DeltaSeconds);
    UpdateBreathing(DeltaSeconds);
    UpdateLookAt(DeltaSeconds);
}

void UAIDigitalHumanAnimInstance::SetAnimationState(EDigitalHumanAnimState NewState)
{
    if (CurrentState != NewState)
    {
        PreviousState = CurrentState;
        CurrentState = NewState;

        UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Animation state changed: %s -> %s"),
            *UEnum::GetValueAsString(PreviousState),
            *UEnum::GetValueAsString(CurrentState));
    }
}

void UAIDigitalHumanAnimInstance::SetEmotion(EDigitalHumanEmotion NewEmotion, float Intensity)
{
    CurrentEmotion = NewEmotion;
    EmotionIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
}

void UAIDigitalHumanAnimInstance::SetLayerWeights(const FAnimationLayerWeights& Weights)
{
    LayerWeights = Weights;
}

void UAIDigitalHumanAnimInstance::SetLayerWeight(EAnimationLayer Layer, float Weight)
{
    Weight = FMath::Clamp(Weight, 0.0f, 1.0f);

    switch (Layer)
    {
    case EAnimationLayer::Base:
        LayerWeights.BaseLayerWeight = Weight;
        break;
    case EAnimationLayer::Idle:
        LayerWeights.IdleLayerWeight = Weight;
        break;
    case EAnimationLayer::Interaction:
        LayerWeights.GestureLayerWeight = Weight;
        break;
    case EAnimationLayer::Facial:
        LayerWeights.FacialLayerWeight = Weight;
        break;
    case EAnimationLayer::Additive:
        LayerWeights.AdditiveLayerWeight = Weight;
        break;
    default:
        break;
    }
}

void UAIDigitalHumanAnimInstance::SetIdleVariation(int32 VariationIndex)
{
    IdleVariationIndex = FMath::Clamp(VariationIndex, 0, NumIdleVariations - 1);
    IdleVariationTimer = 0.0f;
}

void UAIDigitalHumanAnimInstance::SetIdleWeightShift(float Amount)
{
    IdleWeightShift = FMath::Clamp(Amount, -1.0f, 1.0f);
}

void UAIDigitalHumanAnimInstance::SetIsTalking(bool bTalking)
{
    if (bIsTalking != bTalking)
    {
        bIsTalking = bTalking;

        if (bTalking)
        {
            SetAnimationState(EDigitalHumanAnimState::Talking);
        }
        else
        {
            SetAnimationState(EDigitalHumanAnimState::Idle);
        }
    }
}

void UAIDigitalHumanAnimInstance::SetSpeechIntensity(float Intensity)
{
    SpeechIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
}

void UAIDigitalHumanAnimInstance::TriggerGesture(const FString& GestureName)
{
    // In a full implementation, this would trigger a gesture montage
    UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Triggering gesture: %s"), *GestureName);
    SetAnimationState(EDigitalHumanAnimState::Gesturing);
}

void UAIDigitalHumanAnimInstance::SetGestureWeight(float Weight)
{
    LayerWeights.GestureLayerWeight = FMath::Clamp(Weight, 0.0f, 1.0f);
}

void UAIDigitalHumanAnimInstance::SetBreathingParameters(const FBreathingParameters& Params)
{
    BreathingParams = Params;
}

void UAIDigitalHumanAnimInstance::SetBreathingRateMultiplier(float Multiplier)
{
    // Adjust breathing rate (e.g., faster when excited)
    BreathingParams.BreathingRate = 15.0f * FMath::Clamp(Multiplier, 0.5f, 2.0f);
}

void UAIDigitalHumanAnimInstance::SetLookAtTarget(FVector WorldLocation)
{
    LookAtTarget = WorldLocation;
}

void UAIDigitalHumanAnimInstance::SetLookAtWeight(float Weight)
{
    LookAtWeight = FMath::Clamp(Weight, 0.0f, 1.0f);
}

void UAIDigitalHumanAnimInstance::UpdateIdleAnimation(float DeltaTime)
{
    if (CurrentState != EDigitalHumanAnimState::Idle)
    {
        return;
    }

    IdleTimer += DeltaTime;
    IdleVariationTimer += DeltaTime;

    // Periodically change idle variation
    if (IdleVariationTimer >= IdleVariationChangeInterval)
    {
        IdleVariationTimer = 0.0f;
        SetIdleVariation(FMath::RandRange(0, NumIdleVariations - 1));
    }

    // Calculate subtle weight shifting
    float ShiftFrequency = 0.1f; // Very slow shifting
    IdleWeightShift = FMath::Sin(IdleTimer * ShiftFrequency * PI * 2.0f) * 0.3f;
}

void UAIDigitalHumanAnimInstance::UpdateTalkingAnimation(float DeltaTime)
{
    if (!bIsTalking)
    {
        // Smoothly reduce gesture weight when not talking
        TalkingGestureWeight = FMath::FInterpTo(TalkingGestureWeight, 0.0f, DeltaTime, 5.0f);
        return;
    }

    // Increase gesture weight based on speech intensity
    float TargetGestureWeight = SpeechIntensity * GetEmotionGestureIntensity();
    TalkingGestureWeight = FMath::FInterpTo(TalkingGestureWeight, TargetGestureWeight, DeltaTime, 3.0f);

    LayerWeights.GestureLayerWeight = TalkingGestureWeight;
}

void UAIDigitalHumanAnimInstance::UpdateBreathing(float DeltaTime)
{
    // Calculate breathing cycle
    float BreathsPerSecond = BreathingParams.BreathingRate / 60.0f;
    BreathingTimer += DeltaTime * BreathsPerSecond;

    // Breathing phase (0-1, where 0.5 is peak inhale)
    BreathingPhase = FMath::Sin(BreathingTimer * PI * 2.0f) * 0.5f + 0.5f;

    // Calculate chest expansion and shoulder rise
    float BreathAmount = BreathingPhase * BreathingParams.BreathingDepth;

    if (BreathingParams.bIsBreathingHeavy)
    {
        BreathAmount *= 1.5f;
    }

    BreathingParams.ChestExpansion = BreathAmount;
    BreathingParams.ShoulderRise = BreathAmount * 0.3f; // Shoulders rise less than chest
}

void UAIDigitalHumanAnimInstance::UpdateLookAt(float DeltaTime)
{
    if (LookAtWeight <= 0.0f || !OwnerCharacter)
    {
        return;
    }

    // Calculate look-at rotation
    FVector HeadLocation = OwnerCharacter->GetMesh()->GetSocketLocation(FName("head"));
    FVector LookDirection = (LookAtTarget - HeadLocation).GetSafeNormal();

    FRotator TargetRotation = LookDirection.Rotation();
    FRotator CharacterRotation = OwnerCharacter->GetActorRotation();

    // Get relative rotation
    FRotator RelativeRotation = TargetRotation - CharacterRotation;
    RelativeRotation.Normalize();

    // Clamp to realistic head rotation range
    RelativeRotation.Yaw = FMath::Clamp(RelativeRotation.Yaw, -70.0f, 70.0f);
    RelativeRotation.Pitch = FMath::Clamp(RelativeRotation.Pitch, -30.0f, 30.0f);

    // Smooth interpolation
    CurrentHeadRotation = FMath::RInterpTo(CurrentHeadRotation, RelativeRotation, DeltaTime, 5.0f);
}

float UAIDigitalHumanAnimInstance::GetEmotionGestureIntensity() const
{
    switch (CurrentEmotion)
    {
    case EDigitalHumanEmotion::Excited:
        return 1.0f * EmotionIntensity;
    case EDigitalHumanEmotion::Happy:
        return 0.7f * EmotionIntensity;
    case EDigitalHumanEmotion::Surprised:
        return 0.8f * EmotionIntensity;
    case EDigitalHumanEmotion::Angry:
        return 0.6f * EmotionIntensity;
    case EDigitalHumanEmotion::Sad:
        return 0.3f * EmotionIntensity;
    case EDigitalHumanEmotion::Thinking:
        return 0.4f * EmotionIntensity;
    default:
        return 0.5f * EmotionIntensity;
    }
}
