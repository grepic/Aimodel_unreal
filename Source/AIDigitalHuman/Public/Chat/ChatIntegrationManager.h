// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "IWebSocket.h"
#include "ChatIntegrationManager.generated.h"

/**
 * Chat platform configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FChatPlatformConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString PlatformName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString WebSocketURL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString APIKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FString ChannelID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bAutoReconnect = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    float ReconnectDelay = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 MaxReconnectAttempts = 10;
};

/**
 * Command definition for chat commands
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FChatCommand
{
    GENERATED_BODY()

    /** Command trigger (e.g., "!wave") */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    FString Trigger;

    /** Command description */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    FString Description;

    /** Animation to trigger */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    FString AnimationName;

    /** Required user tier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    EUserTier RequiredTier = EUserTier::Free;

    /** Cooldown in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    float CooldownSeconds = 5.0f;

    /** Global cooldown (shared across all users) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    float GlobalCooldownSeconds = 0.0f;

    /** Cost in platform currency (optional) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    int32 Cost = 0;

    /** Whether to trigger AI response */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    bool bTriggerAIResponse = false;
};

/**
 * Intent classification result
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FChatIntent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Intent")
    FString IntentType;

    UPROPERTY(BlueprintReadOnly, Category = "Intent")
    float Confidence = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Intent")
    TMap<FString, FString> Parameters;

    UPROPERTY(BlueprintReadOnly, Category = "Intent")
    bool bIsCommand = false;

    UPROPERTY(BlueprintReadOnly, Category = "Intent")
    FString CommandTrigger;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatConnected, const FString&, Platform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatDisconnected, const FString&, Platform, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageReceived, const FChatMessage&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatCommandExecuted, const FChatCommand&, Command, const FString&, Username);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatIntentClassified, const FChatIntent&, Intent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatError, const FString&, ErrorMessage);

/**
 * Manages live chat integration from streaming platforms.
 * Handles WebSocket connections, message processing, command parsing, and intent classification.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UChatIntegrationManager : public UObject
{
    GENERATED_BODY()

public:
    UChatIntegrationManager();

    /** Initialize the chat manager */
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void Initialize();

    /** Connect to a chat platform */
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void Connect(const FChatPlatformConfig& Config);

    /** Disconnect from current platform */
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void Disconnect();

    /** Check connection status */
    UFUNCTION(BlueprintCallable, Category = "Chat")
    bool IsConnected() const { return bIsConnected; }

    /** Register a chat command */
    UFUNCTION(BlueprintCallable, Category = "Chat|Commands")
    void RegisterCommand(const FChatCommand& Command);

    /** Unregister a command */
    UFUNCTION(BlueprintCallable, Category = "Chat|Commands")
    void UnregisterCommand(const FString& Trigger);

    /** Get all registered commands */
    UFUNCTION(BlueprintCallable, Category = "Chat|Commands")
    TArray<FChatCommand> GetRegisteredCommands() const;

    /** Manually process a message (for testing or external sources) */
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void ProcessMessage(const FChatMessage& Message);

    /** Simulate a message (for testing) */
    UFUNCTION(BlueprintCallable, Category = "Chat|Testing")
    void SimulateMessage(const FString& Username, const FString& Text, EUserTier Tier = EUserTier::Free);

    /** Get message queue size */
    UFUNCTION(BlueprintCallable, Category = "Chat")
    int32 GetQueueSize() const { return MessageQueue.Num(); }

    /** Get current platform name */
    UFUNCTION(BlueprintCallable, Category = "Chat")
    FString GetCurrentPlatform() const { return CurrentConfig.PlatformName; }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Chat|Events")
    FOnChatConnected OnConnected;

    UPROPERTY(BlueprintAssignable, Category = "Chat|Events")
    FOnChatDisconnected OnDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "Chat|Events")
    FOnChatMessageReceived OnMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "Chat|Events")
    FOnChatCommandExecuted OnCommandExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Chat|Events")
    FOnChatIntentClassified OnIntentClassified;

    UPROPERTY(BlueprintAssignable, Category = "Chat|Events")
    FOnChatError OnError;

protected:
    /** Parse incoming WebSocket message */
    void ParseWebSocketMessage(const FString& Message);

    /** Classify message intent */
    FChatIntent ClassifyIntent(const FChatMessage& Message);

    /** Check if message is a command */
    bool TryProcessCommand(const FChatMessage& Message);

    /** Check command cooldowns */
    bool CheckCooldown(const FString& CommandTrigger, const FString& UserID);

    /** Update cooldown timestamps */
    void UpdateCooldown(const FString& CommandTrigger, const FString& UserID);

    /** Handle WebSocket events */
    void OnWebSocketConnected();
    void OnWebSocketConnectionError(const FString& Error);
    void OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
    void OnWebSocketMessage(const FString& Message);

    /** Attempt reconnection */
    void AttemptReconnect();

    /** Send heartbeat/ping */
    void SendHeartbeat();

    /** Add message to priority queue */
    void EnqueueMessage(const FChatMessage& Message);

    /** Process next message from queue */
    void ProcessQueue();

private:
    TSharedPtr<IWebSocket> WebSocket;
    FChatPlatformConfig CurrentConfig;

    // Commands
    TMap<FString, FChatCommand> RegisteredCommands;

    // Cooldowns: CommandTrigger -> (UserID -> LastUsedTime)
    TMap<FString, TMap<FString, FDateTime>> UserCooldowns;
    TMap<FString, FDateTime> GlobalCooldowns;

    // Message queue (priority ordered)
    TArray<FChatMessage> MessageQueue;

    // State
    bool bIsInitialized = false;
    bool bIsConnected = false;
    int32 ReconnectAttempts = 0;

    // Timers
    FTimerHandle HeartbeatTimer;
    FTimerHandle ReconnectTimer;
    FTimerHandle QueueProcessTimer;

    // Rate limiting
    int32 MessagesProcessedThisSecond = 0;
    int32 MaxMessagesPerSecond = 10;
};
