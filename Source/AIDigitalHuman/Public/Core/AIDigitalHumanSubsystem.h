// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/AIDigitalHumanTypes.h"
#include "AIDigitalHumanSubsystem.generated.h"

class UAIConversationManager;
class UVoiceSynthesisManager;
class UChatIntegrationManager;
class UMonetizationManager;
class UStreamingManager;
class UCinematicCameraManager;
class USafetyManager;
class UEnvironmentManager;
class UAdultPersonalitySystem;
class AAIDigitalHumanCharacter;

/**
 * Main subsystem for the AI Digital Human.
 * Coordinates all managers and provides unified API for the system.
 */
UCLASS()
class AIDIGITALHUMAN_API UAIDigitalHumanSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    /** Implement this for initialization, return false to prevent creation */
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

    /** Initialize the subsystem */
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** Cleanup the subsystem */
    virtual void Deinitialize() override;

    // ==================== Manager Accessors ====================

    /** Get the AI Conversation Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UAIConversationManager* GetConversationManager() const { return ConversationManager; }

    /** Get the Voice Synthesis Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UVoiceSynthesisManager* GetVoiceManager() const { return VoiceManager; }

    /** Get the Chat Integration Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UChatIntegrationManager* GetChatManager() const { return ChatManager; }

    /** Get the Monetization Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UMonetizationManager* GetMonetizationManager() const { return MonetizationManager; }

    /** Get the Streaming Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UStreamingManager* GetStreamingManager() const { return StreamingManager; }

    /** Get the Cinematic Camera Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UCinematicCameraManager* GetCameraManager() const { return CameraManager; }

    /** Get the Safety Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    USafetyManager* GetSafetyManager() const { return SafetyManager; }

    /** Get the Environment Manager */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UEnvironmentManager* GetEnvironmentManager() const { return EnvironmentManager; }

    /** Get the Adult Personality System */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    UAdultPersonalitySystem* GetPersonalitySystem() const { return PersonalitySystem; }

    /** Register the character instance */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    void RegisterCharacter(AAIDigitalHumanCharacter* Character);

    /** Get the registered character */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    AAIDigitalHumanCharacter* GetCharacter() const { return RegisteredCharacter; }

    // ==================== Quick Access Methods ====================

    /** Process incoming chat message */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Chat")
    void ProcessChatMessage(const FChatMessage& Message);

    /** Process monetization event */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Monetization")
    void ProcessMonetizationEvent(const FMonetizationEvent& Event);

    /** Make the character speak with emotion */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Voice")
    void Speak(const FString& Text, EDigitalHumanEmotion Emotion = EDigitalHumanEmotion::Neutral);

    /** Stop current speech */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Voice")
    void StopSpeaking();

    /** Trigger an animation by name */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Animation")
    void TriggerAnimation(const FString& AnimationName, float BlendTime = 0.25f);

    /** Set the character's current emotion */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Emotion")
    void SetEmotion(EDigitalHumanEmotion Emotion, float Intensity = 0.5f);

    /** Get current performance metrics */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Performance")
    FPerformanceMetrics GetPerformanceMetrics() const;

    /** Enable/disable streaming */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Streaming")
    void SetStreamingEnabled(bool bEnabled);

    /** Emergency stop - halts all activity */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Safety")
    void EmergencyStop();

    /** Resume after emergency stop */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human|Safety")
    void Resume();

    // ==================== Status ====================

    /** Check if the system is fully initialized and ready */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    bool IsReady() const { return bIsReady; }

    /** Check if streaming is currently active */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    bool IsStreaming() const;

    /** Check if the character is currently speaking */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    bool IsSpeaking() const;

    /** Check if the system is in emergency stop state */
    UFUNCTION(BlueprintCallable, Category = "AI Digital Human")
    bool IsEmergencyStopped() const { return bEmergencyStopped; }

    // ==================== Events ====================

    UPROPERTY(BlueprintAssignable, Category = "AI Digital Human|Events")
    FOnChatMessageReceived OnChatMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "AI Digital Human|Events")
    FOnMonetizationEvent OnMonetizationEventReceived;

    UPROPERTY(BlueprintAssignable, Category = "AI Digital Human|Events")
    FOnAIResponseReady OnAIResponseReady;

    UPROPERTY(BlueprintAssignable, Category = "AI Digital Human|Events")
    FOnEmotionChanged OnEmotionChanged;

    UPROPERTY(BlueprintAssignable, Category = "AI Digital Human|Events")
    FOnSpeechStarted OnSpeechStarted;

    UPROPERTY(BlueprintAssignable, Category = "AI Digital Human|Events")
    FOnSpeechEnded OnSpeechEnded;

    UPROPERTY(BlueprintAssignable, Category = "AI Digital Human|Events")
    FOnPerformanceWarning OnPerformanceWarning;

protected:
    /** Initialize all manager instances */
    void InitializeManagers();

    /** Setup event bindings between managers */
    void SetupEventBindings();

    /** Update performance monitoring */
    void UpdatePerformanceMetrics();

private:
    UPROPERTY()
    UAIConversationManager* ConversationManager;

    UPROPERTY()
    UVoiceSynthesisManager* VoiceManager;

    UPROPERTY()
    UChatIntegrationManager* ChatManager;

    UPROPERTY()
    UMonetizationManager* MonetizationManager;

    UPROPERTY()
    UStreamingManager* StreamingManager;

    UPROPERTY()
    UCinematicCameraManager* CameraManager;

    UPROPERTY()
    USafetyManager* SafetyManager;

    UPROPERTY()
    UEnvironmentManager* EnvironmentManager;

    UPROPERTY()
    UAdultPersonalitySystem* PersonalitySystem;

    UPROPERTY()
    AAIDigitalHumanCharacter* RegisteredCharacter;

    bool bIsReady = false;
    bool bEmergencyStopped = false;

    FTimerHandle PerformanceUpdateTimer;
    FPerformanceMetrics CurrentMetrics;
};
