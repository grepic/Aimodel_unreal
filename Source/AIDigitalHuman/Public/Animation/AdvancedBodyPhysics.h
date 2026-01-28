// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AdvancedBodyPhysics.generated.h"

/**
 * Body physics simulation type
 */
UENUM(BlueprintType)
enum class EBodyPhysicsType : uint8
{
    None            UMETA(DisplayName = "Disabled"),
    Subtle          UMETA(DisplayName = "Subtle (minimal jiggle)"),
    Natural         UMETA(DisplayName = "Natural (realistic)"),
    Enhanced        UMETA(DisplayName = "Enhanced (exaggerated)")
};

/**
 * Individual body part physics configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FBodyPartPhysics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    FName BoneName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    bool bEnabled = true;

    /** Mass of the simulated body part */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float Mass = 1.0f;

    /** Stiffness of spring (higher = less movement) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Stiffness = 0.5f;

    /** Damping (higher = faster settle) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Damping = 0.3f;

    /** Gravity influence */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float GravityScale = 1.0f;

    /** Maximum displacement from rest position */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", ClampMax = "20.0"))
    float MaxDisplacement = 5.0f;

    /** Inertia scale (how much momentum carries) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float InertiaScale = 1.0f;
};

/**
 * Soft body collision configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FSoftBodyCollision
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    FName BoneName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    float CollisionRadius = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    float Softness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    bool bCollideWithHands = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    bool bCollideWithBody = true;
};

/**
 * Breathing animation parameters
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FBreathingConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float BaseRate = 15.0f; // breaths per minute

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float ChestExpansion = 2.0f; // cm

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float ShoulderRise = 0.5f; // cm

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float BellyExpansion = 1.5f; // cm

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    bool bHeavyBreathing = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    float HeavyBreathingMultiplier = 2.0f;
};

/**
 * Skin deformation parameters
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FSkinDeformation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
    bool bEnableMuscleFlexing = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
    bool bEnableSkinSliding = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
    float MuscleIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
    float SkinElasticity = 0.8f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPhysicsImpact, FName, BoneName, float, ImpactForce);

/**
 * Advanced body physics simulation for realistic soft body movement.
 * Handles breast physics, butt physics, belly jiggle, and muscle simulation.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIDIGITALHUMAN_API UAdvancedBodyPhysics : public UActorComponent
{
    GENERATED_BODY()

public:
    UAdvancedBodyPhysics();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ==================== Configuration ====================

    /** Set physics simulation type */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void SetPhysicsType(EBodyPhysicsType Type);

    /** Get current physics type */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    EBodyPhysicsType GetPhysicsType() const { return CurrentPhysicsType; }

    /** Enable/disable all body physics */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void SetEnabled(bool bEnabled);

    /** Configure specific body part */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void ConfigureBodyPart(const FBodyPartPhysics& Config);

    /** Configure breathing */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void SetBreathingConfig(const FBreathingConfig& Config);

    /** Set breathing rate multiplier (for exertion) */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void SetBreathingRateMultiplier(float Multiplier);

    /** Enable heavy breathing */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void SetHeavyBreathing(bool bHeavy, float Intensity = 1.0f);

    // ==================== Runtime Control ====================

    /** Apply impulse to body part */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void ApplyImpulse(FName BoneName, FVector Impulse);

    /** Apply continuous force to body part */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void ApplyForce(FName BoneName, FVector Force);

    /** Reset all physics to rest state */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    void ResetToRestState();

    /** Get current displacement of body part */
    UFUNCTION(BlueprintCallable, Category = "Body Physics")
    FVector GetBodyPartDisplacement(FName BoneName) const;

    // ==================== Presets ====================

    /** Apply preset for specific activity */
    UFUNCTION(BlueprintCallable, Category = "Body Physics|Presets")
    void ApplyActivityPreset(const FString& ActivityName);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Body Physics|Events")
    FOnBodyPhysicsImpact OnBodyPhysicsImpact;

protected:
    /** Initialize physics bones */
    void InitializePhysicsBones();

    /** Update single body part physics */
    void UpdateBodyPartPhysics(FBodyPartPhysics& Part, float DeltaTime);

    /** Update breathing animation */
    void UpdateBreathing(float DeltaTime);

    /** Apply physics results to skeleton */
    void ApplyPhysicsToSkeleton();

    /** Get physics preset for type */
    TArray<FBodyPartPhysics> GetPresetForType(EBodyPhysicsType Type) const;

private:
    UPROPERTY(EditAnywhere, Category = "Body Physics")
    EBodyPhysicsType CurrentPhysicsType = EBodyPhysicsType::Natural;

    UPROPERTY(EditAnywhere, Category = "Body Physics")
    TArray<FBodyPartPhysics> BodyPartConfigs;

    UPROPERTY(EditAnywhere, Category = "Body Physics")
    TArray<FSoftBodyCollision> CollisionConfigs;

    UPROPERTY(EditAnywhere, Category = "Body Physics")
    FBreathingConfig BreathingConfig;

    UPROPERTY(EditAnywhere, Category = "Body Physics")
    FSkinDeformation SkinConfig;

    // Runtime state
    UPROPERTY()
    USkeletalMeshComponent* OwnerMesh;

    TMap<FName, FVector> CurrentDisplacements;
    TMap<FName, FVector> CurrentVelocities;

    float BreathingPhase = 0.0f;
    float BreathingRateMultiplier = 1.0f;

    bool bIsEnabled = true;
};
