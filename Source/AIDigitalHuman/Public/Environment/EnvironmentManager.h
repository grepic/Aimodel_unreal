// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Environment/EnvironmentPreset.h"
#include "EnvironmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnvironmentLoaded, UEnvironmentPreset*, LoadedPreset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnvironmentUnloaded, UEnvironmentPreset*, UnloadedPreset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLightingChanged, ETimeOfDay, NewTime, float, TransitionDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoodChanged, EEnvironmentMood, NewMood);

/**
 * Manages environment loading, transitions, and lighting changes
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UEnvironmentManager : public UObject
{
    GENERATED_BODY()

public:
    UEnvironmentManager();

    /** Initialize the environment manager */
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void Initialize();

    // ==================== Environment Loading ====================

    /** Load an environment preset */
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void LoadEnvironment(UEnvironmentPreset* Preset, float TransitionDuration = 1.0f);

    /** Load environment by type */
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void LoadEnvironmentByType(EEnvironmentType Type, float TransitionDuration = 1.0f);

    /** Get current environment */
    UFUNCTION(BlueprintCallable, Category = "Environment")
    UEnvironmentPreset* GetCurrentEnvironment() const { return CurrentPreset; }

    /** Check if environment is currently loading */
    UFUNCTION(BlueprintCallable, Category = "Environment")
    bool IsLoading() const { return bIsLoading; }

    // ==================== Lighting Control ====================

    /** Set time of day */
    UFUNCTION(BlueprintCallable, Category = "Environment|Lighting")
    void SetTimeOfDay(ETimeOfDay Time, float TransitionDuration = 2.0f);

    /** Get current time of day */
    UFUNCTION(BlueprintCallable, Category = "Environment|Lighting")
    ETimeOfDay GetTimeOfDay() const { return CurrentTimeOfDay; }

    /** Apply lighting preset by name */
    UFUNCTION(BlueprintCallable, Category = "Environment|Lighting")
    void ApplyLightingPreset(const FString& PresetName, float TransitionDuration = 1.0f);

    /** Set custom lighting */
    UFUNCTION(BlueprintCallable, Category = "Environment|Lighting")
    void SetCustomLighting(const FEnvironmentLighting& Lighting, float TransitionDuration = 1.0f);

    // ==================== Mood Control ====================

    /** Set environment mood */
    UFUNCTION(BlueprintCallable, Category = "Environment|Mood")
    void SetMood(EEnvironmentMood Mood, float TransitionDuration = 1.5f);

    /** Get current mood */
    UFUNCTION(BlueprintCallable, Category = "Environment|Mood")
    EEnvironmentMood GetCurrentMood() const { return CurrentMood; }

    // ==================== Props & Interaction ====================

    /** Get available character positions */
    UFUNCTION(BlueprintCallable, Category = "Environment|Character")
    TArray<FTransform> GetCharacterPositions() const;

    /** Move character to position index */
    UFUNCTION(BlueprintCallable, Category = "Environment|Character")
    void MoveCharacterToPosition(int32 PositionIndex, float TransitionDuration = 0.5f);

    /** Get interactive props */
    UFUNCTION(BlueprintCallable, Category = "Environment|Props")
    TArray<FInteractiveProp> GetInteractiveProps() const;

    /** Trigger prop interaction */
    UFUNCTION(BlueprintCallable, Category = "Environment|Props")
    void InteractWithProp(const FString& PropName);

    // ==================== Camera ====================

    /** Get camera presets for current environment */
    UFUNCTION(BlueprintCallable, Category = "Environment|Camera")
    TArray<FEnvironmentCameraPreset> GetCameraPresets() const;

    /** Apply camera preset */
    UFUNCTION(BlueprintCallable, Category = "Environment|Camera")
    void ApplyCameraPreset(const FString& PresetName, float TransitionDuration = 1.0f);

    // ==================== Audio ====================

    /** Start ambient audio */
    UFUNCTION(BlueprintCallable, Category = "Environment|Audio")
    void StartAmbientAudio();

    /** Stop ambient audio */
    UFUNCTION(BlueprintCallable, Category = "Environment|Audio")
    void StopAmbientAudio();

    /** Set ambient volume */
    UFUNCTION(BlueprintCallable, Category = "Environment|Audio")
    void SetAmbientVolume(float Volume);

    // ==================== Events ====================

    UPROPERTY(BlueprintAssignable, Category = "Environment|Events")
    FOnEnvironmentLoaded OnEnvironmentLoaded;

    UPROPERTY(BlueprintAssignable, Category = "Environment|Events")
    FOnEnvironmentUnloaded OnEnvironmentUnloaded;

    UPROPERTY(BlueprintAssignable, Category = "Environment|Events")
    FOnLightingChanged OnLightingChanged;

    UPROPERTY(BlueprintAssignable, Category = "Environment|Events")
    FOnMoodChanged OnMoodChanged;

    // ==================== Preset Registry ====================

    /** Register environment preset */
    UFUNCTION(BlueprintCallable, Category = "Environment|Registry")
    void RegisterPreset(UEnvironmentPreset* Preset);

    /** Get all registered presets */
    UFUNCTION(BlueprintCallable, Category = "Environment|Registry")
    TArray<UEnvironmentPreset*> GetAllPresets() const;

    /** Get preset by type */
    UFUNCTION(BlueprintCallable, Category = "Environment|Registry")
    UEnvironmentPreset* GetPresetByType(EEnvironmentType Type) const;

protected:
    /** Internal environment loading */
    void LoadEnvironmentInternal(UEnvironmentPreset* Preset);

    /** Apply lighting to scene */
    void ApplyLightingToScene(const FEnvironmentLighting& Lighting, float Duration);

    /** Apply mood adjustments */
    void ApplyMoodAdjustments(EEnvironmentMood Mood);

    /** Get lighting for time of day */
    FEnvironmentLighting GetLightingForTimeOfDay(ETimeOfDay Time) const;

    /** Called when level streaming completes */
    void OnLevelStreamingComplete();

private:
    UPROPERTY()
    UEnvironmentPreset* CurrentPreset;

    UPROPERTY()
    TArray<UEnvironmentPreset*> RegisteredPresets;

    ETimeOfDay CurrentTimeOfDay = ETimeOfDay::GoldenHour;
    EEnvironmentMood CurrentMood = EEnvironmentMood::Cozy;

    FEnvironmentLighting CurrentLighting;
    FEnvironmentLighting TargetLighting;

    bool bIsInitialized = false;
    bool bIsLoading = false;

    // Level streaming handle
    FLatentActionInfo LevelStreamingLatentInfo;

    // Audio
    UPROPERTY()
    UAudioComponent* AmbientAudioComponent;

    float AmbientVolume = 0.5f;

    // Transition state
    float LightingTransitionProgress = 1.0f;
    float LightingTransitionDuration = 1.0f;

    FTimerHandle LightingTransitionTimer;
};
