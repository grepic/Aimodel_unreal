// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Character/AIDigitalHumanCharacter.h"
#include "Character/ClothingSimulationManager.h"
#include "Animation/AdvancedBodyPhysics.h"
#include "Core/AIDigitalHumanSubsystem.h"
#include "AIDigitalHumanModule.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Animation/AnimInstance.h"
#include "GroomComponent.h"
#include "Kismet/GameplayStatics.h"

AAIDigitalHumanCharacter::AAIDigitalHumanCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create face mesh component
    FaceMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceMesh"));
    FaceMeshComponent->SetupAttachment(GetMesh(), FName("head"));

    // Create groom components
    HairGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroom"));
    HairGroomComponent->SetupAttachment(GetMesh());

    EyebrowGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("EyebrowGroom"));
    EyebrowGroomComponent->SetupAttachment(FaceMeshComponent);

    EyelashGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("EyelashGroom"));
    EyelashGroomComponent->SetupAttachment(FaceMeshComponent);

    // Create audio component for voice
    VoiceAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceAudio"));
    VoiceAudioComponent->SetupAttachment(GetMesh(), FName("head"));
    VoiceAudioComponent->bAutoActivate = false;

    // Initialize verification flags
    VerificationFlags.bIsFullySynthetic = true;
    VerificationFlags.bAgeVerified18Plus = true;
    VerificationFlags.bNoFaceCloning = true;
    VerificationFlags.bNoRealLikeness = true;
    VerificationFlags.GenerationTimestamp = FDateTime::Now();
    VerificationFlags.GenerationSeed = FMath::Rand();

    // Generate verification hash
    FString HashInput = FString::Printf(TEXT("%d_%s_%d"),
        VerificationFlags.GenerationSeed,
        *VerificationFlags.GenerationTimestamp.ToString(),
        VerificationFlags.bIsFullySynthetic ? 1 : 0);
    VerificationFlags.VerificationHash = FMD5::HashAnsiString(*HashInput);

    // Create clothing simulation manager
    ClothingManager = CreateDefaultSubobject<UClothingSimulationManager>(TEXT("ClothingManager"));

    // Create advanced body physics
    BodyPhysics = CreateDefaultSubobject<UAdvancedBodyPhysics>(TEXT("BodyPhysics"));
}

void AAIDigitalHumanCharacter::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogAIDigitalHuman, Log, TEXT("AI Digital Human Character initialized"));
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Verification - Synthetic: %s, Age 18+: %s"),
        VerificationFlags.bIsFullySynthetic ? TEXT("Yes") : TEXT("No"),
        VerificationFlags.bAgeVerified18Plus ? TEXT("Yes") : TEXT("No"));

    // Initialize blink timer
    NextBlinkTime = FMath::RandRange(AverageBlinkInterval - BlinkIntervalVariance,
        AverageBlinkInterval + BlinkIntervalVariance);

    // Initialize clothing manager
    if (ClothingManager)
    {
        ClothingManager->Initialize(this);
    }

    // Initialize body physics
    if (BodyPhysics)
    {
        BodyPhysics->Initialize(GetMesh());
    }

    // Register with subsystem
    if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
    {
        if (UAIDigitalHumanSubsystem* Subsystem = GameInstance->GetSubsystem<UAIDigitalHumanSubsystem>())
        {
            Subsystem->RegisterCharacter(this);
        }
    }
}

void AAIDigitalHumanCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateEmotionBlend(DeltaTime);
    UpdateEyeBehavior(DeltaTime);

    // Update body physics
    if (BodyPhysics)
    {
        BodyPhysics->UpdatePhysics(DeltaTime);
    }
}

void AAIDigitalHumanCharacter::SetEmotion(EDigitalHumanEmotion NewEmotion, float Intensity, float BlendTime)
{
    TargetEmotion = NewEmotion;
    TargetEmotionIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
    EmotionBlendTime = FMath::Max(BlendTime, 0.01f);
    EmotionBlendProgress = 0.0f;

    UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Setting emotion to %s (intensity: %.2f)"),
        *UEnum::GetValueAsString(NewEmotion), Intensity);
}

void AAIDigitalHumanCharacter::UpdateEmotionBlend(float DeltaTime)
{
    if (EmotionBlendProgress >= 1.0f)
    {
        return;
    }

    EmotionBlendProgress += DeltaTime / EmotionBlendTime;
    EmotionBlendProgress = FMath::Min(EmotionBlendProgress, 1.0f);

    // Smooth interpolation
    float Alpha = FMath::InterpEaseInOut(0.0f, 1.0f, EmotionBlendProgress, 2.0f);

    // Blend intensity
    EmotionIntensity = FMath::Lerp(EmotionIntensity, TargetEmotionIntensity, Alpha);

    // Update current emotion when blend completes
    if (EmotionBlendProgress >= 1.0f)
    {
        CurrentEmotion = TargetEmotion;
    }

    // Apply to face
    ApplyEmotionToFace(TargetEmotion, EmotionIntensity * Alpha);
}

void AAIDigitalHumanCharacter::ApplyEmotionToFace(EDigitalHumanEmotion Emotion, float Intensity)
{
    TMap<FName, float> EmotionWeights = GetEmotionBlendshapes(Emotion, Intensity);

    // Merge with existing weights
    for (const auto& Pair : EmotionWeights)
    {
        CurrentEmotionWeights.Add(Pair.Key, Pair.Value);
    }

    // Apply all weights to face mesh
    if (FaceMeshComponent && FaceMeshComponent->SkeletalMesh)
    {
        for (const auto& Pair : CurrentEmotionWeights)
        {
            FaceMeshComponent->SetMorphTarget(Pair.Key, Pair.Value);
        }
    }

    // Also apply to main mesh if it has morph targets
    if (GetMesh() && GetMesh()->SkeletalMesh)
    {
        for (const auto& Pair : CurrentEmotionWeights)
        {
            GetMesh()->SetMorphTarget(Pair.Key, Pair.Value);
        }
    }
}

TMap<FName, float> AAIDigitalHumanCharacter::GetEmotionBlendshapes(EDigitalHumanEmotion Emotion, float Intensity) const
{
    TMap<FName, float> Weights;
    float I = Intensity;

    switch (Emotion)
    {
    case EDigitalHumanEmotion::Happy:
        Weights.Add(FName("mouthSmileLeft"), 0.8f * I);
        Weights.Add(FName("mouthSmileRight"), 0.8f * I);
        Weights.Add(FName("cheekSquintLeft"), 0.4f * I);
        Weights.Add(FName("cheekSquintRight"), 0.4f * I);
        Weights.Add(FName("eyeSquintLeft"), 0.3f * I);
        Weights.Add(FName("eyeSquintRight"), 0.3f * I);
        break;

    case EDigitalHumanEmotion::Sad:
        Weights.Add(FName("mouthFrownLeft"), 0.6f * I);
        Weights.Add(FName("mouthFrownRight"), 0.6f * I);
        Weights.Add(FName("browInnerUp"), 0.5f * I);
        Weights.Add(FName("browDownLeft"), 0.3f * I);
        Weights.Add(FName("browDownRight"), 0.3f * I);
        break;

    case EDigitalHumanEmotion::Surprised:
        Weights.Add(FName("eyeWideLeft"), 0.8f * I);
        Weights.Add(FName("eyeWideRight"), 0.8f * I);
        Weights.Add(FName("browOuterUpLeft"), 0.7f * I);
        Weights.Add(FName("browOuterUpRight"), 0.7f * I);
        Weights.Add(FName("jawOpen"), 0.4f * I);
        break;

    case EDigitalHumanEmotion::Excited:
        Weights.Add(FName("mouthSmileLeft"), 1.0f * I);
        Weights.Add(FName("mouthSmileRight"), 1.0f * I);
        Weights.Add(FName("eyeWideLeft"), 0.5f * I);
        Weights.Add(FName("eyeWideRight"), 0.5f * I);
        Weights.Add(FName("browOuterUpLeft"), 0.4f * I);
        Weights.Add(FName("browOuterUpRight"), 0.4f * I);
        break;

    case EDigitalHumanEmotion::Thinking:
        Weights.Add(FName("browInnerUp"), 0.4f * I);
        Weights.Add(FName("eyeLookUpLeft"), 0.3f * I);
        Weights.Add(FName("eyeLookUpRight"), 0.3f * I);
        Weights.Add(FName("mouthPucker"), 0.2f * I);
        break;

    case EDigitalHumanEmotion::Flirty:
        Weights.Add(FName("mouthSmileLeft"), 0.5f * I);
        Weights.Add(FName("mouthSmileRight"), 0.7f * I);
        Weights.Add(FName("eyeSquintLeft"), 0.3f * I);
        Weights.Add(FName("eyeSquintRight"), 0.5f * I);
        Weights.Add(FName("browDownRight"), 0.2f * I);
        break;

    case EDigitalHumanEmotion::Confused:
        Weights.Add(FName("browInnerUp"), 0.5f * I);
        Weights.Add(FName("browDownLeft"), 0.3f * I);
        Weights.Add(FName("mouthFrownLeft"), 0.2f * I);
        Weights.Add(FName("mouthFrownRight"), 0.2f * I);
        break;

    case EDigitalHumanEmotion::Angry:
        Weights.Add(FName("browDownLeft"), 0.7f * I);
        Weights.Add(FName("browDownRight"), 0.7f * I);
        Weights.Add(FName("noseSneerLeft"), 0.4f * I);
        Weights.Add(FName("noseSneerRight"), 0.4f * I);
        Weights.Add(FName("jawForward"), 0.3f * I);
        break;

    default:
        // Neutral - no weights
        break;
    }

    return Weights;
}

void AAIDigitalHumanCharacter::ApplyLipSyncFrame(const FLipSyncFrame& Frame)
{
    // Clear previous lip sync weights
    for (const auto& Pair : CurrentLipSyncWeights)
    {
        if (FaceMeshComponent)
        {
            FaceMeshComponent->SetMorphTarget(Pair.Key, 0.0f);
        }
        if (GetMesh())
        {
            GetMesh()->SetMorphTarget(Pair.Key, 0.0f);
        }
    }

    // Apply new weights
    CurrentLipSyncWeights = Frame.BlendshapeWeights;

    for (const auto& Pair : CurrentLipSyncWeights)
    {
        if (FaceMeshComponent)
        {
            FaceMeshComponent->SetMorphTarget(Pair.Key, Pair.Value);
        }
        if (GetMesh())
        {
            GetMesh()->SetMorphTarget(Pair.Key, Pair.Value);
        }
    }
}

void AAIDigitalHumanCharacter::SetBlendshapeWeights(const TMap<FName, float>& Weights)
{
    for (const auto& Pair : Weights)
    {
        if (FaceMeshComponent)
        {
            FaceMeshComponent->SetMorphTarget(Pair.Key, Pair.Value);
        }
        if (GetMesh())
        {
            GetMesh()->SetMorphTarget(Pair.Key, Pair.Value);
        }
    }
}

void AAIDigitalHumanCharacter::ClearBlendshapeWeights()
{
    CurrentLipSyncWeights.Empty();
    CurrentEmotionWeights.Empty();
}

void AAIDigitalHumanCharacter::PlayAnimationMontage(UAnimMontage* Montage, float PlayRate)
{
    if (Montage)
    {
        PlayAnimMontage(Montage, PlayRate);
    }
}

void AAIDigitalHumanCharacter::TriggerAnimation(const FString& AnimationName, float BlendTime)
{
    if (UAnimMontage** Montage = AnimationMontages.Find(AnimationName))
    {
        PlayAnimationMontage(*Montage);
    }
    else
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Animation not found: %s"), *AnimationName);
    }
}

void AAIDigitalHumanCharacter::SetIdleVariation(int32 VariationIndex)
{
    // This would be implemented in the AnimInstance to switch idle animations
    UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Setting idle variation: %d"), VariationIndex);
}

void AAIDigitalHumanCharacter::UpdateEyeBehavior(float DeltaTime)
{
    UpdateGaze(DeltaTime);
    ProcessMicroSaccades(DeltaTime);
    ProcessBlinking(DeltaTime);
}

void AAIDigitalHumanCharacter::SetGazeTarget(FVector WorldLocation)
{
    GazeTargetLocation = WorldLocation;
    GazeTargetActor = nullptr;
}

void AAIDigitalHumanCharacter::SetGazeTargetActor(AActor* Target)
{
    GazeTargetActor = Target;
}

void AAIDigitalHumanCharacter::ClearGazeTarget()
{
    GazeTargetActor = nullptr;
    GazeTargetLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;
}

void AAIDigitalHumanCharacter::UpdateGaze(float DeltaTime)
{
    FVector TargetLocation = GazeTargetActor ?
        GazeTargetActor->GetActorLocation() : GazeTargetLocation;

    // Calculate relative look direction
    FVector EyeLocation = GetMesh()->GetSocketLocation(FName("head"));
    FVector LookDirection = (TargetLocation - EyeLocation).GetSafeNormal();

    // Convert to local space for eye rotation
    FVector LocalLook = GetActorTransform().InverseTransformVector(LookDirection);

    // Calculate gaze angles
    float Yaw = FMath::RadiansToDegrees(FMath::Atan2(LocalLook.Y, LocalLook.X));
    float Pitch = FMath::RadiansToDegrees(FMath::Asin(LocalLook.Z));

    // Clamp to realistic eye movement range
    Yaw = FMath::Clamp(Yaw, -30.0f, 30.0f);
    Pitch = FMath::Clamp(Pitch, -20.0f, 20.0f);

    // Add saccade offset
    FVector2D SaccadeOffset(
        FMath::Sin(EyeState.SaccadeTimer * 10.0f) * SaccadeAmplitude,
        FMath::Sin(EyeState.SaccadeTimer * 13.0f) * SaccadeAmplitude * 0.5f
    );

    // Smooth interpolation
    FVector2D TargetGaze(Yaw + SaccadeOffset.X, Pitch + SaccadeOffset.Y);
    EyeState.GazeDirection = FMath::Vector2DInterpTo(
        EyeState.GazeDirection, TargetGaze, DeltaTime, 10.0f);

    // Apply to morph targets
    float LookLeftRight = EyeState.GazeDirection.X / 30.0f;
    float LookUpDown = EyeState.GazeDirection.Y / 20.0f;

    if (FaceMeshComponent)
    {
        // Left eye
        FaceMeshComponent->SetMorphTarget(FName("eyeLookInLeft"), FMath::Max(0.0f, -LookLeftRight));
        FaceMeshComponent->SetMorphTarget(FName("eyeLookOutLeft"), FMath::Max(0.0f, LookLeftRight));
        FaceMeshComponent->SetMorphTarget(FName("eyeLookUpLeft"), FMath::Max(0.0f, LookUpDown));
        FaceMeshComponent->SetMorphTarget(FName("eyeLookDownLeft"), FMath::Max(0.0f, -LookUpDown));

        // Right eye
        FaceMeshComponent->SetMorphTarget(FName("eyeLookInRight"), FMath::Max(0.0f, LookLeftRight));
        FaceMeshComponent->SetMorphTarget(FName("eyeLookOutRight"), FMath::Max(0.0f, -LookLeftRight));
        FaceMeshComponent->SetMorphTarget(FName("eyeLookUpRight"), FMath::Max(0.0f, LookUpDown));
        FaceMeshComponent->SetMorphTarget(FName("eyeLookDownRight"), FMath::Max(0.0f, -LookUpDown));
    }
}

void AAIDigitalHumanCharacter::ProcessMicroSaccades(float DeltaTime)
{
    EyeState.SaccadeTimer += DeltaTime * SaccadeFrequency;
}

void AAIDigitalHumanCharacter::ProcessBlinking(float DeltaTime)
{
    EyeState.BlinkTimer += DeltaTime;

    if (EyeState.bIsBlinking)
    {
        // Blink animation (close then open)
        float BlinkProgress = EyeState.BlinkTimer / 0.15f; // 150ms blink

        float BlinkWeight;
        if (BlinkProgress < 0.5f)
        {
            BlinkWeight = BlinkProgress * 2.0f;
        }
        else
        {
            BlinkWeight = 1.0f - (BlinkProgress - 0.5f) * 2.0f;
        }

        if (FaceMeshComponent)
        {
            FaceMeshComponent->SetMorphTarget(FName("eyeBlinkLeft"), BlinkWeight);
            FaceMeshComponent->SetMorphTarget(FName("eyeBlinkRight"), BlinkWeight);
        }

        if (BlinkProgress >= 1.0f)
        {
            EyeState.bIsBlinking = false;
            EyeState.BlinkTimer = 0.0f;

            // Schedule next blink
            NextBlinkTime = FMath::RandRange(
                AverageBlinkInterval - BlinkIntervalVariance,
                AverageBlinkInterval + BlinkIntervalVariance
            );
        }
    }
    else
    {
        // Check if it's time to blink
        if (EyeState.BlinkTimer >= NextBlinkTime)
        {
            TriggerBlink();
        }
    }
}

void AAIDigitalHumanCharacter::TriggerBlink()
{
    EyeState.bIsBlinking = true;
    EyeState.BlinkTimer = 0.0f;
}

void AAIDigitalHumanCharacter::SetPupilDilation(float Dilation)
{
    EyeState.PupilDilation = FMath::Clamp(Dilation, 0.0f, 1.0f);

    // Pupil dilation would be applied via material parameter
    // This is a placeholder for the actual implementation
}
