// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Core/AIDigitalHumanSubsystem.h"
#include "AI/AIConversationManager.h"
#include "AI/AdultPersonalitySystem.h"
#include "Voice/VoiceSynthesisManager.h"
#include "Chat/ChatIntegrationManager.h"
#include "Monetization/MonetizationManager.h"
#include "Streaming/StreamingManager.h"
#include "Camera/CinematicCameraManager.h"
#include "Safety/SafetyManager.h"
#include "Environment/EnvironmentManager.h"
#include "Character/AIDigitalHumanCharacter.h"
#include "AIDigitalHumanModule.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UAIDigitalHumanSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing AI Digital Human Subsystem..."));

    InitializeManagers();
    SetupEventBindings();

    // Setup performance monitoring timer
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            PerformanceUpdateTimer,
            this,
            &UAIDigitalHumanSubsystem::UpdatePerformanceMetrics,
            0.5f, // Update every 500ms
            true
        );
    }

    bIsReady = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("AI Digital Human Subsystem Initialized Successfully"));
}

void UAIDigitalHumanSubsystem::Deinitialize()
{
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Deinitializing AI Digital Human Subsystem..."));

    // Clear timer
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(PerformanceUpdateTimer);
    }

    // Cleanup managers
    if (StreamingManager)
    {
        StreamingManager->StopStreaming();
    }

    bIsReady = false;

    Super::Deinitialize();
}

void UAIDigitalHumanSubsystem::InitializeManagers()
{
    // Create all manager instances
    ConversationManager = NewObject<UAIConversationManager>(this);
    VoiceManager = NewObject<UVoiceSynthesisManager>(this);
    ChatManager = NewObject<UChatIntegrationManager>(this);
    MonetizationManager = NewObject<UMonetizationManager>(this);
    StreamingManager = NewObject<UStreamingManager>(this);
    CameraManager = NewObject<UCinematicCameraManager>(this);
    SafetyManager = NewObject<USafetyManager>(this);
    EnvironmentManager = NewObject<UEnvironmentManager>(this);
    PersonalitySystem = NewObject<UAdultPersonalitySystem>(this);

    // Initialize all managers
    if (ConversationManager) ConversationManager->Initialize();
    if (VoiceManager) VoiceManager->Initialize();
    if (ChatManager) ChatManager->Initialize();
    if (MonetizationManager) MonetizationManager->Initialize();
    if (StreamingManager) StreamingManager->Initialize();
    if (CameraManager) CameraManager->Initialize();
    if (SafetyManager) SafetyManager->Initialize();
    if (EnvironmentManager) EnvironmentManager->Initialize();
    if (PersonalitySystem) PersonalitySystem->Initialize();
}

void UAIDigitalHumanSubsystem::RegisterCharacter(AAIDigitalHumanCharacter* Character)
{
    RegisteredCharacter = Character;

    // Initialize personality system with character reference
    if (PersonalitySystem && RegisteredCharacter)
    {
        PersonalitySystem->SetOwnerCharacter(RegisteredCharacter);
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Character registered with subsystem"));
}

void UAIDigitalHumanSubsystem::SetupEventBindings()
{
    // Bind chat messages to conversation manager
    if (ChatManager && ConversationManager)
    {
        ChatManager->OnMessageReceived.AddDynamic(this, &UAIDigitalHumanSubsystem::ProcessChatMessage);
    }

    // Bind monetization events
    if (MonetizationManager)
    {
        MonetizationManager->OnEvent.AddDynamic(this, &UAIDigitalHumanSubsystem::ProcessMonetizationEvent);
    }

    // Bind AI responses to voice synthesis
    if (ConversationManager && VoiceManager)
    {
        ConversationManager->OnResponseReady.AddLambda([this](const FAIResponse& Response)
        {
            if (!bEmergencyStopped)
            {
                Speak(Response.ResponseText, Response.Emotion);
                OnAIResponseReady.Broadcast(Response);
            }
        });
    }
}

void UAIDigitalHumanSubsystem::ProcessChatMessage(const FChatMessage& Message)
{
    if (bEmergencyStopped)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Chat message ignored - system in emergency stop state"));
        return;
    }

    // Check safety first
    if (SafetyManager && !SafetyManager->ValidateMessage(Message))
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Chat message blocked by safety manager"));
        return;
    }

    // Log the interaction
    if (SafetyManager)
    {
        SafetyManager->LogInteraction(TEXT("ChatMessage"), Message.Username, Message.Message);
    }

    // Forward to conversation manager
    if (ConversationManager)
    {
        ConversationManager->ProcessMessage(Message);
    }

    // Broadcast event
    OnChatMessageReceived.Broadcast(Message);
}

void UAIDigitalHumanSubsystem::ProcessMonetizationEvent(const FMonetizationEvent& Event)
{
    if (bEmergencyStopped)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Monetization event ignored - system in emergency stop state"));
        return;
    }

    // Log the interaction
    if (SafetyManager)
    {
        SafetyManager->LogInteraction(TEXT("Monetization"), Event.Username,
            FString::Printf(TEXT("%s: %.2f"), *Event.EventType, Event.Amount));
    }

    // Handle special animations/reactions based on tip amount
    if (Event.EventType == TEXT("Tip"))
    {
        if (Event.Amount >= 100.0f)
        {
            TriggerAnimation(TEXT("BigTipReaction"), 0.3f);
            SetEmotion(EDigitalHumanEmotion::Excited, 0.9f);
        }
        else if (Event.Amount >= 10.0f)
        {
            TriggerAnimation(TEXT("TipReaction"), 0.25f);
            SetEmotion(EDigitalHumanEmotion::Happy, 0.7f);
        }
    }
    else if (Event.EventType == TEXT("Subscription"))
    {
        TriggerAnimation(TEXT("SubscriptionCelebration"), 0.3f);
        SetEmotion(EDigitalHumanEmotion::Excited, 0.8f);
    }

    // Broadcast event
    OnMonetizationEventReceived.Broadcast(Event);
}

void UAIDigitalHumanSubsystem::Speak(const FString& Text, EDigitalHumanEmotion Emotion)
{
    if (bEmergencyStopped || !VoiceManager)
    {
        return;
    }

    SetEmotion(Emotion);
    VoiceManager->Speak(Text, Emotion);
    OnSpeechStarted.Broadcast(Text);
}

void UAIDigitalHumanSubsystem::StopSpeaking()
{
    if (VoiceManager)
    {
        VoiceManager->StopSpeaking();
        OnSpeechEnded.Broadcast();
    }
}

void UAIDigitalHumanSubsystem::TriggerAnimation(const FString& AnimationName, float BlendTime)
{
    if (bEmergencyStopped)
    {
        return;
    }

    // Animation triggering is handled by the character's animation blueprint
    // This method broadcasts an event that the character listens to
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Triggering animation: %s (blend: %.2fs)"), *AnimationName, BlendTime);
}

void UAIDigitalHumanSubsystem::SetEmotion(EDigitalHumanEmotion Emotion, float Intensity)
{
    if (bEmergencyStopped)
    {
        return;
    }

    OnEmotionChanged.Broadcast(Emotion);
}

FPerformanceMetrics UAIDigitalHumanSubsystem::GetPerformanceMetrics() const
{
    return CurrentMetrics;
}

void UAIDigitalHumanSubsystem::SetStreamingEnabled(bool bEnabled)
{
    if (!StreamingManager)
    {
        return;
    }

    if (bEnabled)
    {
        StreamingManager->StartStreaming();
    }
    else
    {
        StreamingManager->StopStreaming();
    }
}

void UAIDigitalHumanSubsystem::EmergencyStop()
{
    UE_LOG(LogAIDigitalHuman, Warning, TEXT("EMERGENCY STOP ACTIVATED"));

    bEmergencyStopped = true;

    // Stop all active processes
    StopSpeaking();

    if (StreamingManager)
    {
        StreamingManager->StopStreaming();
    }

    if (SafetyManager)
    {
        SafetyManager->LogInteraction(TEXT("Emergency"), TEXT("System"), TEXT("Emergency stop activated"));
    }
}

void UAIDigitalHumanSubsystem::Resume()
{
    if (!bEmergencyStopped)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Resuming from emergency stop"));

    bEmergencyStopped = false;

    if (SafetyManager)
    {
        SafetyManager->LogInteraction(TEXT("Emergency"), TEXT("System"), TEXT("System resumed"));
    }
}

bool UAIDigitalHumanSubsystem::IsStreaming() const
{
    return StreamingManager && StreamingManager->IsStreaming();
}

bool UAIDigitalHumanSubsystem::IsSpeaking() const
{
    return VoiceManager && VoiceManager->IsSpeaking();
}

void UAIDigitalHumanSubsystem::UpdatePerformanceMetrics()
{
    // Get current frame stats
    extern ENGINE_API float GAverageFPS;
    CurrentMetrics.AverageFPS = GAverageFPS;
    CurrentMetrics.CurrentFPS = 1.0f / FApp::GetDeltaTime();
    CurrentMetrics.FrameTimeMs = FApp::GetDeltaTime() * 1000.0f;

    // Get GPU/CPU timing from stats
    // Note: Actual implementation would use RHI queries

    // Check for performance issues
    if (CurrentMetrics.CurrentFPS < 55.0f)
    {
        OnPerformanceWarning.Broadcast(
            FString::Printf(TEXT("FPS dropped to %.1f"), CurrentMetrics.CurrentFPS)
        );
    }
}
