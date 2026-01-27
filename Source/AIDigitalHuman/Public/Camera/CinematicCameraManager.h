// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "CinematicCameraManager.generated.h"

/**
 * Camera shot definition
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FCameraShot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    ECameraShotType ShotType = ECameraShotType::MediumShot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    FVector RelativeOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    FRotator RelativeRotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float FieldOfView = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float FocusDistance = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float Aperture = 2.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float BlendTime = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    bool bUseDOF = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    FName FocusBone = FName("head");
};

/**
 * Camera behavior configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FCameraBehavior
{
    GENERATED_BODY()

    /** Enable automatic shot changes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    bool bAutoShotChanges = true;

    /** Minimum time between auto shot changes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    float MinShotDuration = 5.0f;

    /** Maximum time between auto shot changes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    float MaxShotDuration = 15.0f;

    /** Enable subtle camera movement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    bool bSubtleMovement = true;

    /** Subtle movement amplitude */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    float SubtleMovementAmplitude = 2.0f;

    /** Enable focus tracking */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    bool bFocusTracking = true;

    /** Follow subject with smooth lag */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    float FollowLagSpeed = 3.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCameraShotChanged, ECameraShotType, OldShot, ECameraShotType, NewShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraBlendStarted, float, BlendDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraBlendFinished);

/**
 * Manages the cinematic camera system for streaming.
 * Handles dynamic framing, shot selection, and virtual camera output.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UCinematicCameraManager : public UObject
{
    GENERATED_BODY()

public:
    UCinematicCameraManager();

    /** Initialize the camera manager */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void Initialize();

    /** Set the target actor to follow */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetTargetActor(AActor* Target);

    /** Change to a specific shot type */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetShotType(ECameraShotType ShotType, float BlendTime = 1.0f);

    /** Get current shot type */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    ECameraShotType GetCurrentShotType() const { return CurrentShotType; }

    /** Add a custom shot definition */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void AddShotDefinition(const FCameraShot& Shot);

    /** Set camera behavior */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraBehavior(const FCameraBehavior& Behavior);

    /** Get current camera behavior */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    FCameraBehavior GetCameraBehavior() const { return CameraBehavior; }

    /** Force a random shot change */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void TriggerRandomShot();

    /** Enable/disable auto shot changes */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetAutoShotChanges(bool bEnabled);

    /** Get current camera transform */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    FTransform GetCurrentCameraTransform() const { return CurrentTransform; }

    /** Get current camera settings */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    FCameraShot GetCurrentShot() const;

    /** Update camera (call each frame) */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void UpdateCamera(float DeltaTime);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Camera|Events")
    FOnCameraShotChanged OnShotChanged;

    UPROPERTY(BlueprintAssignable, Category = "Camera|Events")
    FOnCameraBlendStarted OnBlendStarted;

    UPROPERTY(BlueprintAssignable, Category = "Camera|Events")
    FOnCameraBlendFinished OnBlendFinished;

protected:
    /** Initialize default shot definitions */
    void InitializeDefaultShots();

    /** Calculate camera transform for a shot */
    FTransform CalculateShotTransform(const FCameraShot& Shot) const;

    /** Apply subtle camera movement */
    void ApplySubtleMovement(float DeltaTime);

    /** Update focus tracking */
    void UpdateFocusTracking(float DeltaTime);

    /** Handle auto shot change timer */
    void OnAutoShotTimer();

private:
    // Shot definitions
    TMap<ECameraShotType, FCameraShot> ShotDefinitions;

    // Current state
    ECameraShotType CurrentShotType = ECameraShotType::MediumShot;
    FTransform CurrentTransform;
    FTransform TargetTransform;
    FTransform BlendStartTransform;

    // Target
    UPROPERTY()
    AActor* TargetActor;

    // Behavior
    FCameraBehavior CameraBehavior;

    // Blending
    bool bIsBlending = false;
    float BlendProgress = 0.0f;
    float BlendDuration = 1.0f;

    // Subtle movement
    float SubtleTime = 0.0f;
    FVector SubtleOffset;

    // Auto shot timer
    FTimerHandle AutoShotTimer;
    float CurrentShotDuration = 0.0f;

    // State
    bool bIsInitialized = false;
};
