// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Animation/AdvancedBodyPhysics.h"
#include "AIDigitalHumanModule.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

UAdvancedBodyPhysics::UAdvancedBodyPhysics()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UAdvancedBodyPhysics::BeginPlay()
{
    Super::BeginPlay();

    // Get owner mesh
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        OwnerMesh = Character->GetMesh();
    }

    InitializePhysicsBones();

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Advanced Body Physics initialized with %d body parts"), BodyPartConfigs.Num());
}

void UAdvancedBodyPhysics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsEnabled || !OwnerMesh)
    {
        return;
    }

    // Update breathing
    UpdateBreathing(DeltaTime);

    // Update each body part physics
    for (FBodyPartPhysics& Part : BodyPartConfigs)
    {
        if (Part.bEnabled)
        {
            UpdateBodyPartPhysics(Part, DeltaTime);
        }
    }

    // Apply results to skeleton
    ApplyPhysicsToSkeleton();
}

void UAdvancedBodyPhysics::InitializePhysicsBones()
{
    if (BodyPartConfigs.Num() == 0)
    {
        // Initialize with default preset
        SetPhysicsType(CurrentPhysicsType);
    }

    // Initialize displacement maps
    for (const FBodyPartPhysics& Part : BodyPartConfigs)
    {
        CurrentDisplacements.Add(Part.BoneName, FVector::ZeroVector);
        CurrentVelocities.Add(Part.BoneName, FVector::ZeroVector);
    }
}

void UAdvancedBodyPhysics::SetPhysicsType(EBodyPhysicsType Type)
{
    CurrentPhysicsType = Type;
    BodyPartConfigs = GetPresetForType(Type);

    // Reinitialize
    CurrentDisplacements.Empty();
    CurrentVelocities.Empty();

    for (const FBodyPartPhysics& Part : BodyPartConfigs)
    {
        CurrentDisplacements.Add(Part.BoneName, FVector::ZeroVector);
        CurrentVelocities.Add(Part.BoneName, FVector::ZeroVector);
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Body physics type set to: %d"), static_cast<int32>(Type));
}

TArray<FBodyPartPhysics> UAdvancedBodyPhysics::GetPresetForType(EBodyPhysicsType Type) const
{
    TArray<FBodyPartPhysics> Preset;

    if (Type == EBodyPhysicsType::None)
    {
        return Preset;
    }

    // Base multipliers for different types
    float StiffnessMultiplier = 1.0f;
    float DampingMultiplier = 1.0f;
    float DisplacementMultiplier = 1.0f;

    switch (Type)
    {
    case EBodyPhysicsType::Subtle:
        StiffnessMultiplier = 1.5f;
        DampingMultiplier = 1.2f;
        DisplacementMultiplier = 0.5f;
        break;
    case EBodyPhysicsType::Natural:
        StiffnessMultiplier = 1.0f;
        DampingMultiplier = 1.0f;
        DisplacementMultiplier = 1.0f;
        break;
    case EBodyPhysicsType::Enhanced:
        StiffnessMultiplier = 0.7f;
        DampingMultiplier = 0.8f;
        DisplacementMultiplier = 1.5f;
        break;
    default:
        break;
    }

    // Breast physics (left)
    FBodyPartPhysics BreastL;
    BreastL.BoneName = FName("breast_l");
    BreastL.Mass = 0.8f;
    BreastL.Stiffness = 0.4f * StiffnessMultiplier;
    BreastL.Damping = 0.3f * DampingMultiplier;
    BreastL.GravityScale = 1.0f;
    BreastL.MaxDisplacement = 4.0f * DisplacementMultiplier;
    BreastL.InertiaScale = 1.2f;
    Preset.Add(BreastL);

    // Breast physics (right)
    FBodyPartPhysics BreastR = BreastL;
    BreastR.BoneName = FName("breast_r");
    Preset.Add(BreastR);

    // Butt physics (left)
    FBodyPartPhysics ButtL;
    ButtL.BoneName = FName("butt_l");
    ButtL.Mass = 1.2f;
    ButtL.Stiffness = 0.5f * StiffnessMultiplier;
    ButtL.Damping = 0.4f * DampingMultiplier;
    ButtL.GravityScale = 0.8f;
    ButtL.MaxDisplacement = 3.0f * DisplacementMultiplier;
    ButtL.InertiaScale = 1.0f;
    Preset.Add(ButtL);

    // Butt physics (right)
    FBodyPartPhysics ButtR = ButtL;
    ButtR.BoneName = FName("butt_r");
    Preset.Add(ButtR);

    // Belly physics
    FBodyPartPhysics Belly;
    Belly.BoneName = FName("spine_02");
    Belly.Mass = 0.5f;
    Belly.Stiffness = 0.6f * StiffnessMultiplier;
    Belly.Damping = 0.5f * DampingMultiplier;
    Belly.GravityScale = 0.5f;
    Belly.MaxDisplacement = 2.0f * DisplacementMultiplier;
    Belly.InertiaScale = 0.8f;
    Preset.Add(Belly);

    // Thigh physics (subtle)
    FBodyPartPhysics ThighL;
    ThighL.BoneName = FName("thigh_l");
    ThighL.Mass = 1.5f;
    ThighL.Stiffness = 0.7f * StiffnessMultiplier;
    ThighL.Damping = 0.5f * DampingMultiplier;
    ThighL.GravityScale = 0.3f;
    ThighL.MaxDisplacement = 1.5f * DisplacementMultiplier;
    ThighL.InertiaScale = 0.6f;
    Preset.Add(ThighL);

    FBodyPartPhysics ThighR = ThighL;
    ThighR.BoneName = FName("thigh_r");
    Preset.Add(ThighR);

    return Preset;
}

void UAdvancedBodyPhysics::SetEnabled(bool bEnabled)
{
    bIsEnabled = bEnabled;

    if (!bEnabled)
    {
        ResetToRestState();
    }
}

void UAdvancedBodyPhysics::ConfigureBodyPart(const FBodyPartPhysics& Config)
{
    for (FBodyPartPhysics& Part : BodyPartConfigs)
    {
        if (Part.BoneName == Config.BoneName)
        {
            Part = Config;
            return;
        }
    }

    // Add new if not found
    BodyPartConfigs.Add(Config);
    CurrentDisplacements.Add(Config.BoneName, FVector::ZeroVector);
    CurrentVelocities.Add(Config.BoneName, FVector::ZeroVector);
}

void UAdvancedBodyPhysics::SetBreathingConfig(const FBreathingConfig& Config)
{
    BreathingConfig = Config;
}

void UAdvancedBodyPhysics::SetBreathingRateMultiplier(float Multiplier)
{
    BreathingRateMultiplier = FMath::Clamp(Multiplier, 0.5f, 3.0f);
}

void UAdvancedBodyPhysics::SetHeavyBreathing(bool bHeavy, float Intensity)
{
    BreathingConfig.bHeavyBreathing = bHeavy;
    BreathingConfig.HeavyBreathingMultiplier = FMath::Clamp(Intensity, 1.0f, 3.0f);
}

void UAdvancedBodyPhysics::UpdateBodyPartPhysics(FBodyPartPhysics& Part, float DeltaTime)
{
    FVector* Displacement = CurrentDisplacements.Find(Part.BoneName);
    FVector* Velocity = CurrentVelocities.Find(Part.BoneName);

    if (!Displacement || !Velocity)
    {
        return;
    }

    // Get acceleration from owner movement
    FVector OwnerAcceleration = FVector::ZeroVector;
    if (AActor* Owner = GetOwner())
    {
        // Approximate acceleration from velocity change
        static TMap<AActor*, FVector> LastVelocities;
        FVector CurrentVel = Owner->GetVelocity();
        FVector* LastVel = LastVelocities.Find(Owner);

        if (LastVel)
        {
            OwnerAcceleration = (CurrentVel - *LastVel) / FMath::Max(DeltaTime, 0.001f);
        }
        LastVelocities.Add(Owner, CurrentVel);
    }

    // Spring-damper physics
    // F = -k*x - c*v + m*a + m*g
    FVector SpringForce = -Part.Stiffness * (*Displacement) * 1000.0f;
    FVector DampingForce = -Part.Damping * (*Velocity) * 100.0f;
    FVector GravityForce = FVector(0, 0, -980.0f) * Part.GravityScale * Part.Mass;
    FVector InertiaForce = -OwnerAcceleration * Part.InertiaScale * Part.Mass;

    FVector TotalForce = SpringForce + DampingForce + GravityForce + InertiaForce;
    FVector Acceleration = TotalForce / Part.Mass;

    // Integrate
    *Velocity += Acceleration * DeltaTime;
    *Displacement += *Velocity * DeltaTime;

    // Clamp displacement
    float MaxDisp = Part.MaxDisplacement;
    if (Displacement->Size() > MaxDisp)
    {
        *Displacement = Displacement->GetSafeNormal() * MaxDisp;
        // Reflect velocity
        *Velocity *= -0.3f;
    }
}

void UAdvancedBodyPhysics::UpdateBreathing(float DeltaTime)
{
    float EffectiveRate = BreathingConfig.BaseRate * BreathingRateMultiplier;
    if (BreathingConfig.bHeavyBreathing)
    {
        EffectiveRate *= BreathingConfig.HeavyBreathingMultiplier;
    }

    // Update phase (0-1 cycle)
    float BreathsPerSecond = EffectiveRate / 60.0f;
    BreathingPhase += DeltaTime * BreathsPerSecond;
    BreathingPhase = FMath::Fmod(BreathingPhase, 1.0f);

    // Breathing curve (smoother inhale, quicker exhale)
    float BreathCurve;
    if (BreathingPhase < 0.4f)
    {
        // Inhale (slower)
        BreathCurve = FMath::Sin(BreathingPhase / 0.4f * PI * 0.5f);
    }
    else
    {
        // Exhale (faster)
        BreathCurve = FMath::Cos((BreathingPhase - 0.4f) / 0.6f * PI * 0.5f);
    }

    float Multiplier = BreathingConfig.bHeavyBreathing ? BreathingConfig.HeavyBreathingMultiplier : 1.0f;

    // Apply to spine bones for chest/belly movement
    // This would be applied to morphs or bone offsets
}

void UAdvancedBodyPhysics::ApplyPhysicsToSkeleton()
{
    if (!OwnerMesh)
    {
        return;
    }

    for (const FBodyPartPhysics& Part : BodyPartConfigs)
    {
        if (!Part.bEnabled)
        {
            continue;
        }

        FVector* Displacement = CurrentDisplacements.Find(Part.BoneName);
        if (!Displacement)
        {
            continue;
        }

        // Apply as bone offset
        int32 BoneIndex = OwnerMesh->GetBoneIndex(Part.BoneName);
        if (BoneIndex != INDEX_NONE)
        {
            FTransform BoneTransform = OwnerMesh->GetBoneTransform(BoneIndex);
            BoneTransform.AddToTranslation(*Displacement);

            // Use modify bone in anim graph would be better
            // This is a simplified approach
        }
    }
}

void UAdvancedBodyPhysics::ApplyImpulse(FName BoneName, FVector Impulse)
{
    FVector* Velocity = CurrentVelocities.Find(BoneName);
    if (Velocity)
    {
        *Velocity += Impulse;

        // Find mass
        for (const FBodyPartPhysics& Part : BodyPartConfigs)
        {
            if (Part.BoneName == BoneName)
            {
                OnBodyPhysicsImpact.Broadcast(BoneName, Impulse.Size() * Part.Mass);
                break;
            }
        }
    }
}

void UAdvancedBodyPhysics::ApplyForce(FName BoneName, FVector Force)
{
    // Forces are applied continuously, so we add to velocity scaled by delta
    // This should be called in tick
    FVector* Velocity = CurrentVelocities.Find(BoneName);
    if (Velocity)
    {
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        *Velocity += Force * DeltaTime;
    }
}

void UAdvancedBodyPhysics::ResetToRestState()
{
    for (auto& Pair : CurrentDisplacements)
    {
        Pair.Value = FVector::ZeroVector;
    }
    for (auto& Pair : CurrentVelocities)
    {
        Pair.Value = FVector::ZeroVector;
    }
}

FVector UAdvancedBodyPhysics::GetBodyPartDisplacement(FName BoneName) const
{
    const FVector* Displacement = CurrentDisplacements.Find(BoneName);
    return Displacement ? *Displacement : FVector::ZeroVector;
}

void UAdvancedBodyPhysics::ApplyActivityPreset(const FString& ActivityName)
{
    if (ActivityName == TEXT("Idle"))
    {
        SetBreathingRateMultiplier(1.0f);
        SetHeavyBreathing(false);
    }
    else if (ActivityName == TEXT("Walking"))
    {
        SetBreathingRateMultiplier(1.2f);
        SetHeavyBreathing(false);
    }
    else if (ActivityName == TEXT("Dancing"))
    {
        SetBreathingRateMultiplier(1.5f);
        SetHeavyBreathing(false);
    }
    else if (ActivityName == TEXT("Exercising"))
    {
        SetBreathingRateMultiplier(2.0f);
        SetHeavyBreathing(true, 1.5f);
    }
    else if (ActivityName == TEXT("Excited"))
    {
        SetBreathingRateMultiplier(1.8f);
        SetHeavyBreathing(true, 1.2f);
    }
    else if (ActivityName == TEXT("Relaxed"))
    {
        SetBreathingRateMultiplier(0.8f);
        SetHeavyBreathing(false);
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Applied activity preset: %s"), *ActivityName);
}
