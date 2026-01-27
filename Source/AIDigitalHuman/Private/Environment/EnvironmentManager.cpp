// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Environment/EnvironmentManager.h"
#include "AIDigitalHumanModule.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/RectLight.h"
#include "Engine/SkyLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"

UEnvironmentManager::UEnvironmentManager()
{
}

void UEnvironmentManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing Environment Manager..."));

    // Create ambient audio component
    if (UWorld* World = GetWorld())
    {
        AmbientAudioComponent = NewObject<UAudioComponent>(World);
        if (AmbientAudioComponent)
        {
            AmbientAudioComponent->bAutoActivate = false;
            AmbientAudioComponent->SetVolumeMultiplier(AmbientVolume);
        }
    }

    bIsInitialized = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Environment Manager Initialized"));
}

void UEnvironmentManager::LoadEnvironment(UEnvironmentPreset* Preset, float TransitionDuration)
{
    if (!Preset)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Cannot load null environment preset"));
        return;
    }

    if (bIsLoading)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Environment already loading, ignoring request"));
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Loading environment: %s"), *Preset->EnvironmentName);

    bIsLoading = true;

    // Unload current if exists
    if (CurrentPreset && CurrentPreset->LevelAsset.IsValid())
    {
        FLatentActionInfo UnloadLatentInfo;
        UnloadLatentInfo.CallbackTarget = this;
        UnloadLatentInfo.UUID = 1;
        UnloadLatentInfo.Linkage = 0;

        UGameplayStatics::UnloadStreamLevel(
            GetWorld(),
            CurrentPreset->LevelAsset.GetAssetName(),
            UnloadLatentInfo,
            false
        );

        OnEnvironmentUnloaded.Broadcast(CurrentPreset);
    }

    // Load new environment
    LoadEnvironmentInternal(Preset);
}

void UEnvironmentManager::LoadEnvironmentByType(EEnvironmentType Type, float TransitionDuration)
{
    UEnvironmentPreset* Preset = GetPresetByType(Type);
    if (Preset)
    {
        LoadEnvironment(Preset, TransitionDuration);
    }
    else
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("No preset registered for environment type: %d"), static_cast<int32>(Type));
    }
}

void UEnvironmentManager::LoadEnvironmentInternal(UEnvironmentPreset* Preset)
{
    if (!Preset->LevelAsset.IsValid())
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Environment preset has no valid level asset"));
        bIsLoading = false;
        return;
    }

    // Setup latent action for level streaming
    LevelStreamingLatentInfo.CallbackTarget = this;
    LevelStreamingLatentInfo.UUID = 2;
    LevelStreamingLatentInfo.Linkage = 0;

    // Load the level
    UGameplayStatics::LoadStreamLevel(
        GetWorld(),
        Preset->LevelAsset.GetAssetName(),
        true,
        true,
        LevelStreamingLatentInfo
    );

    CurrentPreset = Preset;

    // Apply initial settings
    ApplyLightingToScene(Preset->Lighting, 0.5f);
    ApplyMoodAdjustments(Preset->DefaultMood);

    // Start ambient audio
    if (Preset->AmbientSound.IsValid())
    {
        if (AmbientAudioComponent)
        {
            AmbientAudioComponent->SetSound(Preset->AmbientSound.LoadSynchronous());
            StartAmbientAudio();
        }
    }

    bIsLoading = false;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Environment loaded: %s"), *Preset->EnvironmentName);

    OnEnvironmentLoaded.Broadcast(CurrentPreset);
}

void UEnvironmentManager::SetTimeOfDay(ETimeOfDay Time, float TransitionDuration)
{
    if (CurrentTimeOfDay == Time)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Changing time of day to: %s"),
        *UEnum::GetValueAsString(Time));

    CurrentTimeOfDay = Time;

    // Get lighting for this time
    FEnvironmentLighting NewLighting = GetLightingForTimeOfDay(Time);

    // If current preset has specific preset for this time, use it
    if (CurrentPreset)
    {
        FString TimeKey = UEnum::GetValueAsString(Time);
        if (FEnvironmentLighting* PresetLighting = CurrentPreset->LightingPresets.Find(TimeKey))
        {
            NewLighting = *PresetLighting;
        }
    }

    ApplyLightingToScene(NewLighting, TransitionDuration);

    OnLightingChanged.Broadcast(Time, TransitionDuration);
}

FEnvironmentLighting UEnvironmentManager::GetLightingForTimeOfDay(ETimeOfDay Time) const
{
    FEnvironmentLighting Lighting;

    switch (Time)
    {
    case ETimeOfDay::Dawn:
        Lighting.KeyLightTemperature = 3500.0f;
        Lighting.KeyLightIntensity = 5.0f;
        Lighting.FillLightRatio = 0.4f;
        Lighting.AmbientColor = FLinearColor(0.05f, 0.03f, 0.07f);
        break;

    case ETimeOfDay::Morning:
        Lighting.KeyLightTemperature = 5500.0f;
        Lighting.KeyLightIntensity = 10.0f;
        Lighting.FillLightRatio = 0.35f;
        Lighting.AmbientColor = FLinearColor(0.03f, 0.03f, 0.04f);
        break;

    case ETimeOfDay::Noon:
        Lighting.KeyLightTemperature = 6500.0f;
        Lighting.KeyLightIntensity = 15.0f;
        Lighting.FillLightRatio = 0.25f;
        Lighting.AmbientColor = FLinearColor(0.02f, 0.02f, 0.02f);
        break;

    case ETimeOfDay::Afternoon:
        Lighting.KeyLightTemperature = 5800.0f;
        Lighting.KeyLightIntensity = 12.0f;
        Lighting.FillLightRatio = 0.3f;
        Lighting.AmbientColor = FLinearColor(0.02f, 0.02f, 0.03f);
        break;

    case ETimeOfDay::GoldenHour:
        Lighting.KeyLightTemperature = 3200.0f;
        Lighting.KeyLightIntensity = 8.0f;
        Lighting.FillLightRatio = 0.4f;
        Lighting.AmbientColor = FLinearColor(0.04f, 0.02f, 0.01f);
        break;

    case ETimeOfDay::Sunset:
        Lighting.KeyLightTemperature = 2700.0f;
        Lighting.KeyLightIntensity = 6.0f;
        Lighting.FillLightRatio = 0.5f;
        Lighting.AmbientColor = FLinearColor(0.05f, 0.02f, 0.02f);
        break;

    case ETimeOfDay::BlueHour:
        Lighting.KeyLightTemperature = 8000.0f;
        Lighting.KeyLightIntensity = 3.0f;
        Lighting.FillLightRatio = 0.6f;
        Lighting.AmbientColor = FLinearColor(0.01f, 0.02f, 0.05f);
        break;

    case ETimeOfDay::Night:
        Lighting.KeyLightTemperature = 4000.0f;
        Lighting.KeyLightIntensity = 1.0f;
        Lighting.FillLightRatio = 0.8f;
        Lighting.bEnablePracticalLights = true;
        Lighting.AmbientColor = FLinearColor(0.005f, 0.005f, 0.01f);
        break;

    case ETimeOfDay::LateNight:
        Lighting.KeyLightTemperature = 3000.0f;
        Lighting.KeyLightIntensity = 0.5f;
        Lighting.FillLightRatio = 0.9f;
        Lighting.bEnablePracticalLights = true;
        Lighting.AmbientColor = FLinearColor(0.002f, 0.002f, 0.005f);
        break;
    }

    Lighting.TimeOfDay = Time;
    return Lighting;
}

void UEnvironmentManager::ApplyLightingPreset(const FString& PresetName, float TransitionDuration)
{
    if (!CurrentPreset)
    {
        return;
    }

    if (FEnvironmentLighting* Lighting = CurrentPreset->LightingPresets.Find(PresetName))
    {
        ApplyLightingToScene(*Lighting, TransitionDuration);
    }
    else
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Lighting preset not found: %s"), *PresetName);
    }
}

void UEnvironmentManager::SetCustomLighting(const FEnvironmentLighting& Lighting, float TransitionDuration)
{
    ApplyLightingToScene(Lighting, TransitionDuration);
}

void UEnvironmentManager::ApplyLightingToScene(const FEnvironmentLighting& Lighting, float Duration)
{
    TargetLighting = Lighting;
    LightingTransitionDuration = Duration;
    LightingTransitionProgress = 0.0f;

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // Find and update directional light (sun/moon)
    for (TActorIterator<ADirectionalLight> It(World); It; ++It)
    {
        ADirectionalLight* DirLight = *It;
        if (DirLight && DirLight->GetLightComponent())
        {
            UDirectionalLightComponent* LightComp = DirLight->GetLightComponent();
            LightComp->SetIntensity(Lighting.KeyLightIntensity);
            LightComp->SetLightColor(FLinearColor::MakeFromColorTemperature(Lighting.KeyLightTemperature));
        }
    }

    // Find and update sky light
    for (TActorIterator<ASkyLight> It(World); It; ++It)
    {
        ASkyLight* SkyLight = *It;
        if (SkyLight && SkyLight->GetLightComponent())
        {
            USkyLightComponent* SkyComp = SkyLight->GetLightComponent();
            SkyComp->SetIntensity(Lighting.KeyLightIntensity * 0.3f);
            SkyComp->RecaptureSky();
        }
    }

    // Update fog
    for (TActorIterator<AExponentialHeightFog> It(World); It; ++It)
    {
        AExponentialHeightFog* Fog = *It;
        if (Fog && Fog->GetComponent())
        {
            UExponentialHeightFogComponent* FogComp = Fog->GetComponent();
            FogComp->SetVolumetricFog(Lighting.bEnableVolumetricFog);
            FogComp->SetFogDensity(Lighting.FogDensity);
        }
    }

    CurrentLighting = Lighting;
}

void UEnvironmentManager::SetMood(EEnvironmentMood Mood, float TransitionDuration)
{
    if (CurrentMood == Mood)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Changing mood to: %s"),
        *UEnum::GetValueAsString(Mood));

    CurrentMood = Mood;
    ApplyMoodAdjustments(Mood);

    OnMoodChanged.Broadcast(Mood);
}

void UEnvironmentManager::ApplyMoodAdjustments(EEnvironmentMood Mood)
{
    FEnvironmentLighting MoodLighting = CurrentLighting;

    switch (Mood)
    {
    case EEnvironmentMood::Romantic:
        MoodLighting.KeyLightTemperature = FMath::Min(MoodLighting.KeyLightTemperature, 3000.0f);
        MoodLighting.KeyLightIntensity *= 0.6f;
        MoodLighting.bEnablePracticalLights = true;
        break;

    case EEnvironmentMood::Cozy:
        MoodLighting.KeyLightTemperature = FMath::Min(MoodLighting.KeyLightTemperature, 3500.0f);
        MoodLighting.KeyLightIntensity *= 0.7f;
        MoodLighting.bEnablePracticalLights = true;
        break;

    case EEnvironmentMood::Luxurious:
        MoodLighting.KeyLightIntensity *= 0.8f;
        MoodLighting.RimLightIntensity *= 1.5f;
        break;

    case EEnvironmentMood::Playful:
        MoodLighting.KeyLightIntensity *= 1.1f;
        break;

    case EEnvironmentMood::Mysterious:
        MoodLighting.KeyLightIntensity *= 0.4f;
        MoodLighting.bEnableVolumetricFog = true;
        MoodLighting.FogDensity = 0.05f;
        break;

    case EEnvironmentMood::Sensual:
        MoodLighting.KeyLightTemperature = FMath::Min(MoodLighting.KeyLightTemperature, 2700.0f);
        MoodLighting.KeyLightIntensity *= 0.5f;
        MoodLighting.bEnablePracticalLights = true;
        break;

    default:
        break;
    }

    ApplyLightingToScene(MoodLighting, 1.0f);
}

TArray<FTransform> UEnvironmentManager::GetCharacterPositions() const
{
    if (CurrentPreset)
    {
        return CurrentPreset->CharacterPositions;
    }
    return TArray<FTransform>();
}

void UEnvironmentManager::MoveCharacterToPosition(int32 PositionIndex, float TransitionDuration)
{
    if (!CurrentPreset)
    {
        return;
    }

    if (PositionIndex < 0 || PositionIndex >= CurrentPreset->CharacterPositions.Num())
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Invalid position index: %d"), PositionIndex);
        return;
    }

    FTransform TargetTransform = CurrentPreset->CharacterPositions[PositionIndex];

    // Character movement would be handled by the character's movement component
    // This broadcasts an event that the character listens to
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Moving character to position %d"), PositionIndex);
}

TArray<FInteractiveProp> UEnvironmentManager::GetInteractiveProps() const
{
    if (CurrentPreset)
    {
        return CurrentPreset->InteractiveProps;
    }
    return TArray<FInteractiveProp>();
}

void UEnvironmentManager::InteractWithProp(const FString& PropName)
{
    if (!CurrentPreset)
    {
        return;
    }

    for (const FInteractiveProp& Prop : CurrentPreset->InteractiveProps)
    {
        if (Prop.PropName == PropName)
        {
            UE_LOG(LogAIDigitalHuman, Log, TEXT("Interacting with prop: %s"), *PropName);
            // Trigger interaction animation on character
            return;
        }
    }

    UE_LOG(LogAIDigitalHuman, Warning, TEXT("Prop not found: %s"), *PropName);
}

TArray<FEnvironmentCameraPreset> UEnvironmentManager::GetCameraPresets() const
{
    if (CurrentPreset)
    {
        return CurrentPreset->CameraPresets;
    }
    return TArray<FEnvironmentCameraPreset>();
}

void UEnvironmentManager::ApplyCameraPreset(const FString& PresetName, float TransitionDuration)
{
    if (!CurrentPreset)
    {
        return;
    }

    for (const FEnvironmentCameraPreset& Preset : CurrentPreset->CameraPresets)
    {
        if (Preset.PresetName == PresetName)
        {
            UE_LOG(LogAIDigitalHuman, Log, TEXT("Applying camera preset: %s"), *PresetName);
            // Camera would be moved by the CinematicCameraManager
            return;
        }
    }
}

void UEnvironmentManager::StartAmbientAudio()
{
    if (AmbientAudioComponent && !AmbientAudioComponent->IsPlaying())
    {
        AmbientAudioComponent->FadeIn(2.0f, AmbientVolume);
    }
}

void UEnvironmentManager::StopAmbientAudio()
{
    if (AmbientAudioComponent && AmbientAudioComponent->IsPlaying())
    {
        AmbientAudioComponent->FadeOut(2.0f, 0.0f);
    }
}

void UEnvironmentManager::SetAmbientVolume(float Volume)
{
    AmbientVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

    if (AmbientAudioComponent)
    {
        AmbientAudioComponent->SetVolumeMultiplier(AmbientVolume);
    }
}

void UEnvironmentManager::RegisterPreset(UEnvironmentPreset* Preset)
{
    if (Preset && !RegisteredPresets.Contains(Preset))
    {
        RegisteredPresets.Add(Preset);
        UE_LOG(LogAIDigitalHuman, Log, TEXT("Registered environment preset: %s"), *Preset->EnvironmentName);
    }
}

TArray<UEnvironmentPreset*> UEnvironmentManager::GetAllPresets() const
{
    return RegisteredPresets;
}

UEnvironmentPreset* UEnvironmentManager::GetPresetByType(EEnvironmentType Type) const
{
    for (UEnvironmentPreset* Preset : RegisteredPresets)
    {
        if (Preset && Preset->Type == Type)
        {
            return Preset;
        }
    }
    return nullptr;
}

void UEnvironmentManager::OnLevelStreamingComplete()
{
    bIsLoading = false;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Level streaming complete"));
}
