// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnvironmentPreset.generated.h"

/**
 * Environment type categories
 */
UENUM(BlueprintType)
enum class EEnvironmentType : uint8
{
    Bedroom         UMETA(DisplayName = "Bedroom"),
    LivingRoom      UMETA(DisplayName = "Living Room"),
    Bathroom        UMETA(DisplayName = "Bathroom/Spa"),
    Kitchen         UMETA(DisplayName = "Kitchen"),
    Office          UMETA(DisplayName = "Office/Studio"),
    Balcony         UMETA(DisplayName = "Balcony/Terrace"),
    Garden          UMETA(DisplayName = "Garden"),
    Beach           UMETA(DisplayName = "Beach"),
    Forest          UMETA(DisplayName = "Forest"),
    Pool            UMETA(DisplayName = "Pool Area"),
    Penthouse       UMETA(DisplayName = "Penthouse"),
    Yacht           UMETA(DisplayName = "Yacht"),
    HotTub          UMETA(DisplayName = "Hot Tub/Jacuzzi")
};

/**
 * Time of day for lighting
 */
UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
    Dawn            UMETA(DisplayName = "Dawn (6:00)"),
    Morning         UMETA(DisplayName = "Morning (9:00)"),
    Noon            UMETA(DisplayName = "Noon (12:00)"),
    Afternoon       UMETA(DisplayName = "Afternoon (15:00)"),
    GoldenHour      UMETA(DisplayName = "Golden Hour (18:00)"),
    Sunset          UMETA(DisplayName = "Sunset (19:30)"),
    BlueHour        UMETA(DisplayName = "Blue Hour (20:00)"),
    Night           UMETA(DisplayName = "Night (22:00)"),
    LateNight       UMETA(DisplayName = "Late Night (2:00)")
};

/**
 * Mood/atmosphere setting
 */
UENUM(BlueprintType)
enum class EEnvironmentMood : uint8
{
    Romantic        UMETA(DisplayName = "Romantic"),
    Cozy            UMETA(DisplayName = "Cozy/Intimate"),
    Luxurious       UMETA(DisplayName = "Luxurious"),
    Playful         UMETA(DisplayName = "Playful"),
    Mysterious      UMETA(DisplayName = "Mysterious"),
    Relaxed         UMETA(DisplayName = "Relaxed"),
    Energetic       UMETA(DisplayName = "Energetic"),
    Sensual         UMETA(DisplayName = "Sensual")
};

/**
 * Lighting configuration for environment
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FEnvironmentLighting
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    ETimeOfDay TimeOfDay = ETimeOfDay::GoldenHour;

    /** Key light color temperature (Kelvin) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    float KeyLightTemperature = 5500.0f;

    /** Key light intensity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    float KeyLightIntensity = 10.0f;

    /** Fill light ratio (0-1, relative to key) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    float FillLightRatio = 0.3f;

    /** Rim/back light intensity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    float RimLightIntensity = 5.0f;

    /** Enable volumetric fog */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    bool bEnableVolumetricFog = false;

    /** Fog density */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    float FogDensity = 0.02f;

    /** Enable candles/practical lights */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    bool bEnablePracticalLights = true;

    /** Ambient light color */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    FLinearColor AmbientColor = FLinearColor(0.02f, 0.02f, 0.03f);
};

/**
 * Interactive prop in environment
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FInteractiveProp
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
    FString PropName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
    FTransform DefaultTransform;

    /** Animation to play when interacting */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
    FString InteractionAnimation;

    /** Can character sit/lie on this */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
    bool bIsSurface = false;

    /** Position offsets for sitting/lying */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
    TArray<FTransform> SurfacePositions;
};

/**
 * Camera position preset for environment
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FEnvironmentCameraPreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    FString PresetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    FTransform CameraTransform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float FocalLength = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float Aperture = 2.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    ECameraShotType ShotType = ECameraShotType::MediumShot;
};

/**
 * Complete environment preset data asset
 */
UCLASS(BlueprintType)
class AIDIGITALHUMAN_API UEnvironmentPreset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString EnvironmentName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    EEnvironmentType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    EEnvironmentMood DefaultMood = EEnvironmentMood::Cozy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString Description;

    /** Level to load for this environment */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    TSoftObjectPtr<UWorld> LevelAsset;

    /** Lighting configuration */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    FEnvironmentLighting Lighting;

    /** Available lighting presets */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    TMap<FString, FEnvironmentLighting> LightingPresets;

    /** Character spawn/default position */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FTransform CharacterSpawnTransform;

    /** Available positions for character */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    TArray<FTransform> CharacterPositions;

    /** Interactive props */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Props")
    TArray<FInteractiveProp> InteractiveProps;

    /** Camera presets */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    TArray<FEnvironmentCameraPreset> CameraPresets;

    /** Ambient sound cue */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> AmbientSound;

    /** Music playlist for this environment */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TArray<TSoftObjectPtr<USoundBase>> MusicPlaylist;

    /** Post process volume settings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess")
    FPostProcessSettings PostProcessSettings;
};
