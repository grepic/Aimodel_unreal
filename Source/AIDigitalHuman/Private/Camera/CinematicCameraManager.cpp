// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Camera/CinematicCameraManager.h"
#include "AIDigitalHumanModule.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

UCinematicCameraManager::UCinematicCameraManager()
{
    CameraBehavior.bAutoShotChanges = true;
    CameraBehavior.MinShotDuration = 5.0f;
    CameraBehavior.MaxShotDuration = 15.0f;
    CameraBehavior.bSubtleMovement = true;
    CameraBehavior.SubtleMovementAmplitude = 2.0f;
    CameraBehavior.bFocusTracking = true;
    CameraBehavior.FollowLagSpeed = 3.0f;
}

void UCinematicCameraManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing Cinematic Camera Manager..."));

    InitializeDefaultShots();

    bIsInitialized = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Cinematic Camera Manager Initialized"));
}

void UCinematicCameraManager::InitializeDefaultShots()
{
    // Close Up - face only
    FCameraShot CloseUp;
    CloseUp.ShotType = ECameraShotType::CloseUp;
    CloseUp.RelativeOffset = FVector(-80.0f, 0.0f, 10.0f);
    CloseUp.FieldOfView = 35.0f;
    CloseUp.FocusDistance = 80.0f;
    CloseUp.Aperture = 1.8f;
    CloseUp.bUseDOF = true;
    CloseUp.FocusBone = FName("head");
    ShotDefinitions.Add(ECameraShotType::CloseUp, CloseUp);

    // Medium Close Up - head and shoulders
    FCameraShot MediumCloseUp;
    MediumCloseUp.ShotType = ECameraShotType::MediumCloseUp;
    MediumCloseUp.RelativeOffset = FVector(-120.0f, 0.0f, 5.0f);
    MediumCloseUp.FieldOfView = 40.0f;
    MediumCloseUp.FocusDistance = 120.0f;
    MediumCloseUp.Aperture = 2.0f;
    MediumCloseUp.bUseDOF = true;
    MediumCloseUp.FocusBone = FName("head");
    ShotDefinitions.Add(ECameraShotType::MediumCloseUp, MediumCloseUp);

    // Medium Shot - waist up
    FCameraShot MediumShot;
    MediumShot.ShotType = ECameraShotType::MediumShot;
    MediumShot.RelativeOffset = FVector(-180.0f, 0.0f, 0.0f);
    MediumShot.FieldOfView = 50.0f;
    MediumShot.FocusDistance = 180.0f;
    MediumShot.Aperture = 2.8f;
    MediumShot.bUseDOF = true;
    MediumShot.FocusBone = FName("spine_03");
    ShotDefinitions.Add(ECameraShotType::MediumShot, MediumShot);

    // Medium Wide Shot - knees up
    FCameraShot MediumWideShot;
    MediumWideShot.ShotType = ECameraShotType::MediumWideShot;
    MediumWideShot.RelativeOffset = FVector(-250.0f, 0.0f, -10.0f);
    MediumWideShot.FieldOfView = 55.0f;
    MediumWideShot.FocusDistance = 250.0f;
    MediumWideShot.Aperture = 4.0f;
    MediumWideShot.bUseDOF = true;
    MediumWideShot.FocusBone = FName("pelvis");
    ShotDefinitions.Add(ECameraShotType::MediumWideShot, MediumWideShot);

    // Wide Shot - full body
    FCameraShot WideShot;
    WideShot.ShotType = ECameraShotType::WideShot;
    WideShot.RelativeOffset = FVector(-350.0f, 0.0f, -20.0f);
    WideShot.FieldOfView = 60.0f;
    WideShot.FocusDistance = 350.0f;
    WideShot.Aperture = 5.6f;
    WideShot.bUseDOF = false;
    ShotDefinitions.Add(ECameraShotType::WideShot, WideShot);

    // Low Angle
    FCameraShot LowAngle;
    LowAngle.ShotType = ECameraShotType::LowAngle;
    LowAngle.RelativeOffset = FVector(-150.0f, 0.0f, -60.0f);
    LowAngle.RelativeRotation = FRotator(-15.0f, 0.0f, 0.0f);
    LowAngle.FieldOfView = 45.0f;
    LowAngle.FocusDistance = 150.0f;
    LowAngle.Aperture = 2.8f;
    LowAngle.bUseDOF = true;
    LowAngle.FocusBone = FName("spine_03");
    ShotDefinitions.Add(ECameraShotType::LowAngle, LowAngle);

    // High Angle
    FCameraShot HighAngle;
    HighAngle.ShotType = ECameraShotType::HighAngle;
    HighAngle.RelativeOffset = FVector(-150.0f, 0.0f, 80.0f);
    HighAngle.RelativeRotation = FRotator(15.0f, 0.0f, 0.0f);
    HighAngle.FieldOfView = 45.0f;
    HighAngle.FocusDistance = 150.0f;
    HighAngle.Aperture = 2.8f;
    HighAngle.bUseDOF = true;
    HighAngle.FocusBone = FName("head");
    ShotDefinitions.Add(ECameraShotType::HighAngle, HighAngle);

    // Over The Shoulder
    FCameraShot OTS;
    OTS.ShotType = ECameraShotType::OverTheShoulder;
    OTS.RelativeOffset = FVector(-100.0f, 40.0f, 20.0f);
    OTS.RelativeRotation = FRotator(0.0f, -10.0f, 0.0f);
    OTS.FieldOfView = 50.0f;
    OTS.FocusDistance = 100.0f;
    OTS.Aperture = 2.0f;
    OTS.bUseDOF = true;
    OTS.FocusBone = FName("head");
    ShotDefinitions.Add(ECameraShotType::OverTheShoulder, OTS);

    // Dutch Angle
    FCameraShot Dutch;
    Dutch.ShotType = ECameraShotType::Dutch;
    Dutch.RelativeOffset = FVector(-140.0f, 20.0f, 10.0f);
    Dutch.RelativeRotation = FRotator(0.0f, 0.0f, 10.0f);
    Dutch.FieldOfView = 45.0f;
    Dutch.FocusDistance = 140.0f;
    Dutch.Aperture = 2.8f;
    Dutch.bUseDOF = true;
    Dutch.FocusBone = FName("head");
    ShotDefinitions.Add(ECameraShotType::Dutch, Dutch);
}

void UCinematicCameraManager::SetTargetActor(AActor* Target)
{
    TargetActor = Target;

    if (TargetActor)
    {
        // Calculate initial transform
        CurrentTransform = CalculateShotTransform(GetCurrentShot());
        TargetTransform = CurrentTransform;

        // Start auto shot timer if enabled
        if (CameraBehavior.bAutoShotChanges)
        {
            SetAutoShotChanges(true);
        }
    }
}

void UCinematicCameraManager::SetShotType(ECameraShotType ShotType, float InBlendTime)
{
    if (ShotType == CurrentShotType && !bIsBlending)
    {
        return;
    }

    ECameraShotType OldShot = CurrentShotType;
    CurrentShotType = ShotType;

    // Setup blend
    BlendStartTransform = CurrentTransform;
    TargetTransform = CalculateShotTransform(GetCurrentShot());
    BlendDuration = InBlendTime;
    BlendProgress = 0.0f;
    bIsBlending = true;

    // Reset shot timer
    CurrentShotDuration = 0.0f;

    OnBlendStarted.Broadcast(BlendDuration);
    OnShotChanged.Broadcast(OldShot, CurrentShotType);

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Camera shot changed to: %s"), *UEnum::GetValueAsString(ShotType));
}

FCameraShot UCinematicCameraManager::GetCurrentShot() const
{
    if (const FCameraShot* Shot = ShotDefinitions.Find(CurrentShotType))
    {
        return *Shot;
    }

    // Return default medium shot
    return ShotDefinitions.FindRef(ECameraShotType::MediumShot);
}

void UCinematicCameraManager::AddShotDefinition(const FCameraShot& Shot)
{
    ShotDefinitions.Add(Shot.ShotType, Shot);
}

void UCinematicCameraManager::SetCameraBehavior(const FCameraBehavior& Behavior)
{
    CameraBehavior = Behavior;

    SetAutoShotChanges(Behavior.bAutoShotChanges);
}

void UCinematicCameraManager::TriggerRandomShot()
{
    // Get array of shot types
    TArray<ECameraShotType> AvailableShots;
    ShotDefinitions.GetKeys(AvailableShots);

    // Remove current shot from options
    AvailableShots.Remove(CurrentShotType);

    if (AvailableShots.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailableShots.Num() - 1);
        SetShotType(AvailableShots[RandomIndex], 1.5f);
    }
}

void UCinematicCameraManager::SetAutoShotChanges(bool bEnabled)
{
    CameraBehavior.bAutoShotChanges = bEnabled;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(AutoShotTimer);

        if (bEnabled)
        {
            float Duration = FMath::RandRange(CameraBehavior.MinShotDuration, CameraBehavior.MaxShotDuration);
            World->GetTimerManager().SetTimer(
                AutoShotTimer,
                this,
                &UCinematicCameraManager::OnAutoShotTimer,
                Duration,
                false
            );
        }
    }
}

void UCinematicCameraManager::OnAutoShotTimer()
{
    if (CameraBehavior.bAutoShotChanges)
    {
        TriggerRandomShot();

        // Schedule next shot change
        if (UWorld* World = GetWorld())
        {
            float Duration = FMath::RandRange(CameraBehavior.MinShotDuration, CameraBehavior.MaxShotDuration);
            World->GetTimerManager().SetTimer(
                AutoShotTimer,
                this,
                &UCinematicCameraManager::OnAutoShotTimer,
                Duration,
                false
            );
        }
    }
}

void UCinematicCameraManager::UpdateCamera(float DeltaTime)
{
    if (!TargetActor)
    {
        return;
    }

    // Update shot duration
    CurrentShotDuration += DeltaTime;

    // Handle blend
    if (bIsBlending)
    {
        BlendProgress += DeltaTime / BlendDuration;

        if (BlendProgress >= 1.0f)
        {
            BlendProgress = 1.0f;
            bIsBlending = false;
            CurrentTransform = TargetTransform;
            OnBlendFinished.Broadcast();
        }
        else
        {
            // Smooth blend using ease in-out
            float Alpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendProgress, 2.0f);
            CurrentTransform.Blend(BlendStartTransform, TargetTransform, Alpha);
        }
    }
    else
    {
        // Apply subtle movement
        if (CameraBehavior.bSubtleMovement)
        {
            ApplySubtleMovement(DeltaTime);
        }

        // Update focus tracking
        if (CameraBehavior.bFocusTracking)
        {
            UpdateFocusTracking(DeltaTime);
        }
    }
}

FTransform UCinematicCameraManager::CalculateShotTransform(const FCameraShot& Shot) const
{
    if (!TargetActor)
    {
        return FTransform::Identity;
    }

    FVector TargetLocation = TargetActor->GetActorLocation();
    FRotator TargetRotation = TargetActor->GetActorRotation();

    // Calculate camera position relative to target
    FVector CameraOffset = TargetRotation.RotateVector(Shot.RelativeOffset);
    FVector CameraLocation = TargetLocation + CameraOffset;

    // Calculate look-at rotation
    FVector LookDirection = TargetLocation - CameraLocation;
    FRotator CameraRotation = LookDirection.Rotation();

    // Apply relative rotation
    CameraRotation += Shot.RelativeRotation;

    return FTransform(CameraRotation, CameraLocation);
}

void UCinematicCameraManager::ApplySubtleMovement(float DeltaTime)
{
    SubtleTime += DeltaTime;

    // Create subtle breathing-like movement
    float Amplitude = CameraBehavior.SubtleMovementAmplitude;

    SubtleOffset.X = FMath::Sin(SubtleTime * 0.5f) * Amplitude * 0.1f;
    SubtleOffset.Y = FMath::Sin(SubtleTime * 0.7f) * Amplitude * 0.15f;
    SubtleOffset.Z = FMath::Sin(SubtleTime * 0.3f) * Amplitude * 0.2f;

    // Apply offset to current transform
    FVector CurrentLocation = TargetTransform.GetLocation() + SubtleOffset;
    CurrentTransform.SetLocation(CurrentLocation);
}

void UCinematicCameraManager::UpdateFocusTracking(float DeltaTime)
{
    if (!TargetActor)
    {
        return;
    }

    // Smooth follow the target
    FVector DesiredLocation = CalculateShotTransform(GetCurrentShot()).GetLocation();
    FVector CurrentLocation = CurrentTransform.GetLocation();

    FVector NewLocation = FMath::VInterpTo(CurrentLocation, DesiredLocation, DeltaTime, CameraBehavior.FollowLagSpeed);
    CurrentTransform.SetLocation(NewLocation);

    // Update look-at rotation
    FVector LookTarget = TargetActor->GetActorLocation();
    FVector LookDirection = LookTarget - NewLocation;
    FRotator NewRotation = FMath::RInterpTo(CurrentTransform.Rotator(), LookDirection.Rotation(), DeltaTime, CameraBehavior.FollowLagSpeed * 2.0f);
    CurrentTransform.SetRotation(NewRotation.Quaternion());
}
