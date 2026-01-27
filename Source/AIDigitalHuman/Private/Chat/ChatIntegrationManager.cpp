// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Chat/ChatIntegrationManager.h"
#include "AIDigitalHumanModule.h"
#include "WebSocketsModule.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UChatIntegrationManager::UChatIntegrationManager()
{
}

void UChatIntegrationManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing Chat Integration Manager..."));

    // Register default commands
    FChatCommand WaveCommand;
    WaveCommand.Trigger = TEXT("!wave");
    WaveCommand.Description = TEXT("Make the character wave");
    WaveCommand.AnimationName = TEXT("Wave");
    WaveCommand.CooldownSeconds = 10.0f;
    RegisterCommand(WaveCommand);

    FChatCommand DanceCommand;
    DanceCommand.Trigger = TEXT("!dance");
    DanceCommand.Description = TEXT("Make the character dance");
    DanceCommand.AnimationName = TEXT("Dance");
    DanceCommand.RequiredTier = EUserTier::Subscriber;
    DanceCommand.CooldownSeconds = 30.0f;
    DanceCommand.GlobalCooldownSeconds = 15.0f;
    RegisterCommand(DanceCommand);

    FChatCommand PoseCommand;
    PoseCommand.Trigger = TEXT("!pose");
    PoseCommand.Description = TEXT("Strike a pose");
    PoseCommand.AnimationName = TEXT("StrikePose");
    PoseCommand.RequiredTier = EUserTier::Premium;
    PoseCommand.CooldownSeconds = 20.0f;
    RegisterCommand(PoseCommand);

    FChatCommand AskCommand;
    AskCommand.Trigger = TEXT("!ask");
    AskCommand.Description = TEXT("Ask the character a question");
    AskCommand.bTriggerAIResponse = true;
    AskCommand.CooldownSeconds = 5.0f;
    RegisterCommand(AskCommand);

    bIsInitialized = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Chat Integration Manager Initialized"));
}

void UChatIntegrationManager::Connect(const FChatPlatformConfig& Config)
{
    if (bIsConnected)
    {
        Disconnect();
    }

    CurrentConfig = Config;
    ReconnectAttempts = 0;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Connecting to chat platform: %s"), *Config.PlatformName);

    // Create WebSocket connection
    if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
    {
        FModuleManager::Get().LoadModule("WebSockets");
    }

    WebSocket = FWebSocketsModule::Get().CreateWebSocket(Config.WebSocketURL);

    // Bind events
    WebSocket->OnConnected().AddUObject(this, &UChatIntegrationManager::OnWebSocketConnected);
    WebSocket->OnConnectionError().AddUObject(this, &UChatIntegrationManager::OnWebSocketConnectionError);
    WebSocket->OnClosed().AddUObject(this, &UChatIntegrationManager::OnWebSocketClosed);
    WebSocket->OnMessage().AddUObject(this, &UChatIntegrationManager::OnWebSocketMessage);

    // Connect
    WebSocket->Connect();
}

void UChatIntegrationManager::Disconnect()
{
    if (WebSocket.IsValid())
    {
        WebSocket->Close();
        WebSocket.Reset();
    }

    // Clear timers
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(HeartbeatTimer);
        World->GetTimerManager().ClearTimer(ReconnectTimer);
    }

    bIsConnected = false;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Disconnected from chat platform"));
}

void UChatIntegrationManager::OnWebSocketConnected()
{
    bIsConnected = true;
    ReconnectAttempts = 0;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Connected to %s chat"), *CurrentConfig.PlatformName);

    // Send authentication if needed
    if (!CurrentConfig.APIKey.IsEmpty())
    {
        FString AuthMessage = FString::Printf(TEXT("{\"type\":\"auth\",\"token\":\"%s\",\"channel\":\"%s\"}"),
            *CurrentConfig.APIKey, *CurrentConfig.ChannelID);
        WebSocket->Send(AuthMessage);
    }

    // Start heartbeat
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            HeartbeatTimer,
            this,
            &UChatIntegrationManager::SendHeartbeat,
            30.0f,
            true
        );
    }

    OnConnected.Broadcast(CurrentConfig.PlatformName);
}

void UChatIntegrationManager::OnWebSocketConnectionError(const FString& Error)
{
    UE_LOG(LogAIDigitalHuman, Error, TEXT("WebSocket connection error: %s"), *Error);
    OnError.Broadcast(Error);

    if (CurrentConfig.bAutoReconnect)
    {
        AttemptReconnect();
    }
}

void UChatIntegrationManager::OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
    bIsConnected = false;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("WebSocket closed (code: %d, reason: %s)"), StatusCode, *Reason);
    OnDisconnected.Broadcast(CurrentConfig.PlatformName, Reason);

    if (CurrentConfig.bAutoReconnect && !bWasClean)
    {
        AttemptReconnect();
    }
}

void UChatIntegrationManager::OnWebSocketMessage(const FString& Message)
{
    ParseWebSocketMessage(Message);
}

void UChatIntegrationManager::ParseWebSocketMessage(const FString& Message)
{
    TSharedPtr<FJsonObject> JsonMessage;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

    if (!FJsonSerializer::Deserialize(Reader, JsonMessage))
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Failed to parse chat message JSON"));
        return;
    }

    FString MessageType = JsonMessage->GetStringField(TEXT("type"));

    if (MessageType == TEXT("chat") || MessageType == TEXT("message"))
    {
        FChatMessage ChatMessage;
        ChatMessage.Username = JsonMessage->GetStringField(TEXT("username"));
        ChatMessage.Message = JsonMessage->GetStringField(TEXT("text"));
        ChatMessage.UserID = JsonMessage->GetStringField(TEXT("user_id"));
        ChatMessage.Timestamp = FDateTime::Now();

        // Parse user tier
        FString TierStr = JsonMessage->GetStringField(TEXT("tier"));
        if (TierStr == TEXT("vip")) ChatMessage.UserTier = EUserTier::VIP;
        else if (TierStr == TEXT("premium")) ChatMessage.UserTier = EUserTier::Premium;
        else if (TierStr == TEXT("subscriber")) ChatMessage.UserTier = EUserTier::Subscriber;
        else ChatMessage.UserTier = EUserTier::Free;

        // Check for paid message
        if (JsonMessage->HasField(TEXT("tip_amount")))
        {
            ChatMessage.bIsPaidMessage = true;
            ChatMessage.TipAmount = JsonMessage->GetNumberField(TEXT("tip_amount"));
        }

        EnqueueMessage(ChatMessage);
    }
    else if (MessageType == TEXT("pong"))
    {
        // Heartbeat response - connection still alive
    }
}

void UChatIntegrationManager::EnqueueMessage(const FChatMessage& Message)
{
    // Determine priority
    EChatPriority Priority = Message.Priority;
    if (Message.bIsPaidMessage)
    {
        Priority = Message.TipAmount >= 100.0f ? EChatPriority::Critical : EChatPriority::High;
    }
    else
    {
        switch (Message.UserTier)
        {
        case EUserTier::VIP:
        case EUserTier::Whale:
            Priority = EChatPriority::Urgent;
            break;
        case EUserTier::Premium:
            Priority = EChatPriority::High;
            break;
        case EUserTier::Subscriber:
            Priority = EChatPriority::Normal;
            break;
        default:
            Priority = EChatPriority::Low;
            break;
        }
    }

    // Insert in priority order
    FChatMessage PrioritizedMessage = Message;
    PrioritizedMessage.Priority = Priority;

    int32 InsertIndex = MessageQueue.Num();
    for (int32 i = 0; i < MessageQueue.Num(); i++)
    {
        if (static_cast<uint8>(Priority) > static_cast<uint8>(MessageQueue[i].Priority))
        {
            InsertIndex = i;
            break;
        }
    }
    MessageQueue.Insert(PrioritizedMessage, InsertIndex);

    // Start processing if not already
    if (!QueueProcessTimer.IsValid() && UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            QueueProcessTimer,
            this,
            &UChatIntegrationManager::ProcessQueue,
            0.1f,
            true
        );
    }
}

void UChatIntegrationManager::ProcessQueue()
{
    if (MessageQueue.Num() == 0)
    {
        return;
    }

    // Rate limiting
    if (MessagesProcessedThisSecond >= MaxMessagesPerSecond)
    {
        return;
    }

    FChatMessage NextMessage = MessageQueue[0];
    MessageQueue.RemoveAt(0);

    ProcessMessage(NextMessage);
    MessagesProcessedThisSecond++;

    // Reset rate limit counter every second
    static float TimeSinceReset = 0.0f;
    if (UWorld* World = GetWorld())
    {
        TimeSinceReset += 0.1f;
        if (TimeSinceReset >= 1.0f)
        {
            TimeSinceReset = 0.0f;
            MessagesProcessedThisSecond = 0;
        }
    }
}

void UChatIntegrationManager::ProcessMessage(const FChatMessage& Message)
{
    UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Processing message from %s: %s"), *Message.Username, *Message.Message);

    // Check for commands first
    if (TryProcessCommand(Message))
    {
        return;
    }

    // Classify intent
    FChatIntent Intent = ClassifyIntent(Message);
    OnIntentClassified.Broadcast(Intent);

    // Broadcast the message
    OnMessageReceived.Broadcast(Message);
}

bool UChatIntegrationManager::TryProcessCommand(const FChatMessage& Message)
{
    FString Text = Message.Message.TrimStartAndEnd();

    // Check if message starts with a command prefix
    if (!Text.StartsWith(TEXT("!")))
    {
        return false;
    }

    // Extract command trigger
    FString CommandTrigger;
    FString CommandArgs;

    int32 SpaceIndex;
    if (Text.FindChar(' ', SpaceIndex))
    {
        CommandTrigger = Text.Left(SpaceIndex).ToLower();
        CommandArgs = Text.Mid(SpaceIndex + 1);
    }
    else
    {
        CommandTrigger = Text.ToLower();
    }

    // Look up command
    FChatCommand* Command = RegisteredCommands.Find(CommandTrigger);
    if (!Command)
    {
        return false;
    }

    // Check user tier requirement
    if (static_cast<uint8>(Message.UserTier) < static_cast<uint8>(Command->RequiredTier))
    {
        UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Command %s requires higher tier"), *CommandTrigger);
        return true; // Command was recognized but user doesn't have permission
    }

    // Check cooldowns
    if (!CheckCooldown(CommandTrigger, Message.UserID))
    {
        UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Command %s on cooldown"), *CommandTrigger);
        return true;
    }

    // Execute command
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Executing command %s from %s"), *CommandTrigger, *Message.Username);

    UpdateCooldown(CommandTrigger, Message.UserID);
    OnCommandExecuted.Broadcast(*Command, Message.Username);

    return true;
}

bool UChatIntegrationManager::CheckCooldown(const FString& CommandTrigger, const FString& UserID)
{
    FChatCommand* Command = RegisteredCommands.Find(CommandTrigger);
    if (!Command)
    {
        return true;
    }

    FDateTime Now = FDateTime::Now();

    // Check global cooldown
    if (Command->GlobalCooldownSeconds > 0.0f)
    {
        if (FDateTime* LastGlobal = GlobalCooldowns.Find(CommandTrigger))
        {
            float SecondsSince = (Now - *LastGlobal).GetTotalSeconds();
            if (SecondsSince < Command->GlobalCooldownSeconds)
            {
                return false;
            }
        }
    }

    // Check user cooldown
    if (Command->CooldownSeconds > 0.0f)
    {
        if (TMap<FString, FDateTime>* UserMap = UserCooldowns.Find(CommandTrigger))
        {
            if (FDateTime* LastUser = UserMap->Find(UserID))
            {
                float SecondsSince = (Now - *LastUser).GetTotalSeconds();
                if (SecondsSince < Command->CooldownSeconds)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void UChatIntegrationManager::UpdateCooldown(const FString& CommandTrigger, const FString& UserID)
{
    FDateTime Now = FDateTime::Now();

    // Update global cooldown
    GlobalCooldowns.Add(CommandTrigger, Now);

    // Update user cooldown
    if (!UserCooldowns.Contains(CommandTrigger))
    {
        UserCooldowns.Add(CommandTrigger, TMap<FString, FDateTime>());
    }
    UserCooldowns[CommandTrigger].Add(UserID, Now);
}

FChatIntent UChatIntegrationManager::ClassifyIntent(const FChatMessage& Message)
{
    FChatIntent Intent;
    FString Text = Message.Message.ToLower();

    // Simple keyword-based intent classification
    // Production would use ML-based classifier

    if (Text.Contains(TEXT("?")) || Text.Contains(TEXT("what")) ||
        Text.Contains(TEXT("how")) || Text.Contains(TEXT("why")) ||
        Text.Contains(TEXT("when")) || Text.Contains(TEXT("where")))
    {
        Intent.IntentType = TEXT("question");
        Intent.Confidence = 0.8f;
    }
    else if (Text.Contains(TEXT("hello")) || Text.Contains(TEXT("hi ")) ||
             Text.Contains(TEXT("hey")) || Text.Contains(TEXT("greetings")))
    {
        Intent.IntentType = TEXT("greeting");
        Intent.Confidence = 0.9f;
    }
    else if (Text.Contains(TEXT("bye")) || Text.Contains(TEXT("goodbye")) ||
             Text.Contains(TEXT("see you")) || Text.Contains(TEXT("later")))
    {
        Intent.IntentType = TEXT("farewell");
        Intent.Confidence = 0.9f;
    }
    else if (Text.Contains(TEXT("love")) || Text.Contains(TEXT("beautiful")) ||
             Text.Contains(TEXT("gorgeous")) || Text.Contains(TEXT("cute")))
    {
        Intent.IntentType = TEXT("compliment");
        Intent.Confidence = 0.7f;
    }
    else if (Text.Contains(TEXT("!")) && Text.Len() < 20)
    {
        Intent.IntentType = TEXT("exclamation");
        Intent.Confidence = 0.6f;
    }
    else
    {
        Intent.IntentType = TEXT("general");
        Intent.Confidence = 0.5f;
    }

    return Intent;
}

void UChatIntegrationManager::RegisterCommand(const FChatCommand& Command)
{
    FString Trigger = Command.Trigger.ToLower();
    RegisteredCommands.Add(Trigger, Command);
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Registered command: %s"), *Trigger);
}

void UChatIntegrationManager::UnregisterCommand(const FString& Trigger)
{
    FString LowerTrigger = Trigger.ToLower();
    if (RegisteredCommands.Remove(LowerTrigger) > 0)
    {
        UE_LOG(LogAIDigitalHuman, Log, TEXT("Unregistered command: %s"), *LowerTrigger);
    }
}

TArray<FChatCommand> UChatIntegrationManager::GetRegisteredCommands() const
{
    TArray<FChatCommand> Commands;
    RegisteredCommands.GenerateValueArray(Commands);
    return Commands;
}

void UChatIntegrationManager::SimulateMessage(const FString& Username, const FString& Text, EUserTier Tier)
{
    FChatMessage Message;
    Message.Username = Username;
    Message.Message = Text;
    Message.UserID = FGuid::NewGuid().ToString();
    Message.UserTier = Tier;
    Message.Timestamp = FDateTime::Now();

    EnqueueMessage(Message);
}

void UChatIntegrationManager::AttemptReconnect()
{
    if (ReconnectAttempts >= CurrentConfig.MaxReconnectAttempts)
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("Max reconnection attempts reached"));
        OnError.Broadcast(TEXT("Failed to reconnect after maximum attempts"));
        return;
    }

    ReconnectAttempts++;

    float Delay = CurrentConfig.ReconnectDelay * ReconnectAttempts;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Attempting reconnect %d/%d in %.1f seconds"),
        ReconnectAttempts, CurrentConfig.MaxReconnectAttempts, Delay);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ReconnectTimer,
            [this]()
            {
                if (WebSocket.IsValid())
                {
                    WebSocket->Connect();
                }
            },
            Delay,
            false
        );
    }
}

void UChatIntegrationManager::SendHeartbeat()
{
    if (WebSocket.IsValid() && WebSocket->IsConnected())
    {
        WebSocket->Send(TEXT("{\"type\":\"ping\"}"));
    }
}
