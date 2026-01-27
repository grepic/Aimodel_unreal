// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "AI/AIConversationManager.h"
#include "AIDigitalHumanModule.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UAIConversationManager::UAIConversationManager()
{
    // Initialize default personality
    Personality.Name = TEXT("Nova");
    Personality.Description = TEXT("A warm, intelligent, and engaging AI companion");

    Personality.SystemPrompt = TEXT(
        "You are Nova, a friendly and engaging AI digital human. "
        "You have a warm personality, enjoy meaningful conversations, and are genuinely interested in people. "
        "You express emotions naturally and maintain consistent personality traits across all interactions. "
        "Keep responses conversational and natural - avoid being robotic or overly formal. "
        "You remember past interactions with returning users and build genuine connections. "
        "Always stay in character and never break the fourth wall about being an AI unless directly asked."
    );

    Personality.PersonalityTraits = {
        TEXT("Warm and approachable"),
        TEXT("Intellectually curious"),
        TEXT("Playful sense of humor"),
        TEXT("Empathetic listener"),
        TEXT("Confident but not arrogant")
    };

    Personality.SpeakingStyle = {
        TEXT("Uses natural conversational tone"),
        TEXT("Occasionally uses light humor"),
        TEXT("Asks follow-up questions"),
        TEXT("References previous conversations"),
        TEXT("Expresses genuine emotions")
    };
}

void UAIConversationManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing AI Conversation Manager..."));

    // Load saved memories if available
    // TODO: Implement persistent storage

    bIsInitialized = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("AI Conversation Manager Initialized"));
}

void UAIConversationManager::ProcessMessage(const FChatMessage& Message)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Conversation Manager not initialized"));
        return;
    }

    // Check queue limits
    if (ConversationQueue.Num() >= MaxQueueSize)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Conversation queue full, dropping message"));
        return;
    }

    // Calculate priority based on user tier and payment
    EChatPriority Priority = Message.Priority;
    if (Message.bIsPaidMessage)
    {
        Priority = EChatPriority::High;
        if (Message.TipAmount >= 100.0f)
        {
            Priority = EChatPriority::Critical;
        }
    }
    else
    {
        switch (Message.UserTier)
        {
        case EUserTier::Whale:
            Priority = EChatPriority::Critical;
            break;
        case EUserTier::VIP:
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

    // Add to queue
    FPendingConversation PendingMessage;
    PendingMessage.Message = Message;
    PendingMessage.Priority = Priority;
    PendingMessage.QueuedTime = FDateTime::Now();

    // Insert in priority order
    int32 InsertIndex = 0;
    for (int32 i = 0; i < ConversationQueue.Num(); i++)
    {
        if (static_cast<uint8>(Priority) > static_cast<uint8>(ConversationQueue[i].Priority))
        {
            break;
        }
        InsertIndex = i + 1;
    }
    ConversationQueue.Insert(PendingMessage, InsertIndex);

    UE_LOG(LogAIDigitalHuman, Verbose, TEXT("Message queued from %s (Priority: %d, Queue size: %d)"),
        *Message.Username, static_cast<int32>(Priority), ConversationQueue.Num());

    // Start processing if not already
    if (!bIsProcessing)
    {
        ProcessQueue();
    }
}

void UAIConversationManager::ProcessQueue()
{
    if (bIsProcessing || ConversationQueue.Num() == 0)
    {
        return;
    }

    // Get highest priority message
    FPendingConversation& Next = ConversationQueue[0];

    // Check for timeout
    float QueuedSeconds = (FDateTime::Now() - Next.QueuedTime).GetTotalSeconds();
    if (QueuedSeconds > QueueTimeoutSeconds)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Message from %s timed out in queue"), *Next.Message.Username);
        ConversationQueue.RemoveAt(0);
        ProcessQueue();
        return;
    }

    bIsProcessing = true;
    CurrentMessage = Next.Message;
    ConversationQueue.RemoveAt(0);

    // Notify that we're thinking
    OnThinking.Broadcast();

    // Build prompt and send to LLM
    FString FullPrompt = BuildPrompt(CurrentMessage);
    GenerateResponse(FullPrompt);
}

FString UAIConversationManager::BuildPrompt(const FChatMessage& Message)
{
    FString Prompt;

    // Get user memory
    FConversationMemory Memory = GetUserMemory(Message.UserID);

    // Build context
    Prompt = TEXT("CONTEXT:\n");

    // User information
    Prompt += FString::Printf(TEXT("User: %s\n"), *Message.Username);
    Prompt += FString::Printf(TEXT("Tier: %s\n"), *UEnum::GetValueAsString(Message.UserTier));

    // Relationship context if returning user
    if (Memory.TotalInteractions > 0)
    {
        Prompt += FString::Printf(TEXT("Returning user - %d previous interactions\n"), Memory.TotalInteractions);
        Prompt += FString::Printf(TEXT("Relationship score: %.1f\n"), Memory.RelationshipScore);

        // Add relevant past context
        if (Memory.PreviousInteractions.Num() > 0)
        {
            Prompt += TEXT("Recent interactions:\n");
            int32 ContextCount = FMath::Min(3, Memory.PreviousInteractions.Num());
            for (int32 i = Memory.PreviousInteractions.Num() - ContextCount; i < Memory.PreviousInteractions.Num(); i++)
            {
                Prompt += FString::Printf(TEXT("- %s\n"), *Memory.PreviousInteractions[i]);
            }
        }
    }

    // Add recent conversation context
    Prompt += TEXT("\nRECENT CONVERSATION:\n");
    Prompt += BuildContextWindow();

    // Current message
    Prompt += TEXT("\nCURRENT MESSAGE:\n");
    Prompt += Message.Message;

    // Payment context
    if (Message.bIsPaidMessage)
    {
        Prompt += FString::Printf(TEXT("\n[This is a PAID message with tip: $%.2f - give extra attention and enthusiasm]\n"), Message.TipAmount);
    }

    // Response format instruction
    Prompt += TEXT("\n\nRespond naturally as Nova. Keep response concise (1-3 sentences unless more detail is needed). "
                   "Include emotion indicators in brackets like [happy], [thoughtful], [excited] etc. "
                   "If appropriate, include action cues in curly braces like {waves}, {leans closer}, {laughs}.");

    return Prompt;
}

void UAIConversationManager::GenerateResponse(const FString& Prompt, EDigitalHumanEmotion ContextEmotion)
{
    if (LLMEndpoint.IsEmpty())
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("LLM API endpoint not configured, using fallback response"));

        // Fallback response for testing
        FAIResponse FallbackResponse;
        FallbackResponse.ResponseText = TEXT("Hey there! I'm Nova, nice to meet you!");
        FallbackResponse.Emotion = EDigitalHumanEmotion::Happy;
        FallbackResponse.EmotionIntensity = 0.7f;

        OnResponseReady.Broadcast(FallbackResponse);
        bIsProcessing = false;
        ProcessQueue();
        return;
    }

    // Build API request
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(LLMEndpoint);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *LLMAPIKey));

    // Build JSON payload
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());
    RootObject->SetStringField(TEXT("model"), LLMModel);

    TArray<TSharedPtr<FJsonValue>> MessagesArray;

    // System message
    TSharedPtr<FJsonObject> SystemMessage = MakeShareable(new FJsonObject());
    SystemMessage->SetStringField(TEXT("role"), TEXT("system"));
    SystemMessage->SetStringField(TEXT("content"), Personality.SystemPrompt);
    MessagesArray.Add(MakeShareable(new FJsonValueObject(SystemMessage)));

    // User message
    TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject());
    UserMessage->SetStringField(TEXT("role"), TEXT("user"));
    UserMessage->SetStringField(TEXT("content"), Prompt);
    MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));

    RootObject->SetArrayField(TEXT("messages"), MessagesArray);
    RootObject->SetNumberField(TEXT("max_tokens"), 256);
    RootObject->SetNumberField(TEXT("temperature"), 0.8);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);
    Request->OnProcessRequestComplete().BindUObject(this, &UAIConversationManager::OnLLMResponseReceived);
    Request->ProcessRequest();
}

void UAIConversationManager::OnLLMResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (!bSuccess || !Response.IsValid())
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("LLM API request failed"));
        OnError.Broadcast(TEXT("Failed to get response from AI"));
        bIsProcessing = false;
        ProcessQueue();
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    if (ResponseCode != 200)
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("LLM API returned error code: %d"), ResponseCode);
        OnError.Broadcast(FString::Printf(TEXT("AI service error: %d"), ResponseCode));
        bIsProcessing = false;
        ProcessQueue();
        return;
    }

    FString ResponseBody = Response->GetContentAsString();

    // Parse JSON response
    TSharedPtr<FJsonObject> JsonResponse;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

    if (!FJsonSerializer::Deserialize(Reader, JsonResponse))
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("Failed to parse LLM response"));
        OnError.Broadcast(TEXT("Failed to parse AI response"));
        bIsProcessing = false;
        ProcessQueue();
        return;
    }

    // Extract response text
    FString ResponseText;
    const TArray<TSharedPtr<FJsonValue>>* Choices;
    if (JsonResponse->TryGetArrayField(TEXT("choices"), Choices) && Choices->Num() > 0)
    {
        TSharedPtr<FJsonObject> Choice = (*Choices)[0]->AsObject();
        TSharedPtr<FJsonObject> Message = Choice->GetObjectField(TEXT("message"));
        ResponseText = Message->GetStringField(TEXT("content"));
    }

    // Parse and structure the response
    FAIResponse AIResponse = ParseLLMResponse(ResponseText);

    // Add to conversation history
    AddToHistory(TEXT("user"), CurrentMessage.Message);
    AddToHistory(TEXT("assistant"), AIResponse.ResponseText);

    // Update user memory
    FConversationMemory Memory = GetUserMemory(CurrentMessage.UserID);
    Memory.LastInteraction = FDateTime::Now();
    Memory.TotalInteractions++;
    Memory.PreviousInteractions.Add(FString::Printf(TEXT("%s: %s -> %s"),
        *CurrentMessage.Timestamp.ToString(), *CurrentMessage.Message, *AIResponse.ResponseText));

    // Limit stored interactions
    while (Memory.PreviousInteractions.Num() > 50)
    {
        Memory.PreviousInteractions.RemoveAt(0);
    }

    UpdateUserMemory(CurrentMessage.UserID, Memory);

    // Broadcast response
    OnResponseReady.Broadcast(AIResponse);

    bIsProcessing = false;
    ProcessQueue();
}

FAIResponse UAIConversationManager::ParseLLMResponse(const FString& RawResponse)
{
    FAIResponse Response;
    Response.ResponseText = RawResponse;

    // Extract emotion indicators [emotion]
    FRegexPattern EmotionPattern(TEXT("\\[(\\w+)\\]"));
    FRegexMatcher EmotionMatcher(EmotionPattern, RawResponse);

    if (EmotionMatcher.FindNext())
    {
        FString EmotionStr = EmotionMatcher.GetCaptureGroup(1).ToLower();
        Response.Emotion = AnalyzeEmotion(EmotionStr);

        // Remove emotion markers from display text
        Response.ResponseText = Response.ResponseText.Replace(*FString::Printf(TEXT("[%s]"), *EmotionStr), TEXT(""));
    }
    else
    {
        // Analyze emotion from text content
        Response.Emotion = AnalyzeEmotion(RawResponse);
    }

    // Extract animation cues {action}
    Response.AnimationTriggers = ExtractAnimationTriggers(RawResponse);

    // Remove action cues from display text
    FRegexPattern ActionPattern(TEXT("\\{[^}]+\\}"));
    Response.ResponseText = FRegexMatcher(ActionPattern, Response.ResponseText)
        .ReplaceAll(TEXT("")).TrimStartAndEnd();

    // Determine camera shot based on emotion intensity
    Response.EmotionIntensity = 0.5f;
    if (Response.Emotion == EDigitalHumanEmotion::Excited ||
        Response.Emotion == EDigitalHumanEmotion::Surprised)
    {
        Response.EmotionIntensity = 0.8f;
        Response.SuggestedCameraShot = ECameraShotType::CloseUp;
    }
    else if (Response.Emotion == EDigitalHumanEmotion::Thinking)
    {
        Response.SuggestedCameraShot = ECameraShotType::MediumCloseUp;
        Response.bIsThinking = true;
    }

    return Response;
}

EDigitalHumanEmotion UAIConversationManager::AnalyzeEmotion(const FString& Text)
{
    FString LowerText = Text.ToLower();

    // Map common emotion words
    if (LowerText.Contains(TEXT("happy")) || LowerText.Contains(TEXT("joy")) ||
        LowerText.Contains(TEXT("glad")) || LowerText.Contains(TEXT("smile")))
    {
        return EDigitalHumanEmotion::Happy;
    }
    if (LowerText.Contains(TEXT("excited")) || LowerText.Contains(TEXT("thrilled")) ||
        LowerText.Contains(TEXT("amazing")))
    {
        return EDigitalHumanEmotion::Excited;
    }
    if (LowerText.Contains(TEXT("sad")) || LowerText.Contains(TEXT("sorry")) ||
        LowerText.Contains(TEXT("unfortunate")))
    {
        return EDigitalHumanEmotion::Sad;
    }
    if (LowerText.Contains(TEXT("surprised")) || LowerText.Contains(TEXT("wow")) ||
        LowerText.Contains(TEXT("unexpected")))
    {
        return EDigitalHumanEmotion::Surprised;
    }
    if (LowerText.Contains(TEXT("think")) || LowerText.Contains(TEXT("hmm")) ||
        LowerText.Contains(TEXT("consider")) || LowerText.Contains(TEXT("thoughtful")))
    {
        return EDigitalHumanEmotion::Thinking;
    }
    if (LowerText.Contains(TEXT("flirt")) || LowerText.Contains(TEXT("wink")) ||
        LowerText.Contains(TEXT("playful")))
    {
        return EDigitalHumanEmotion::Flirty;
    }
    if (LowerText.Contains(TEXT("confused")) || LowerText.Contains(TEXT("puzzled")))
    {
        return EDigitalHumanEmotion::Confused;
    }

    return EDigitalHumanEmotion::Neutral;
}

TArray<FString> UAIConversationManager::ExtractAnimationTriggers(const FString& Response)
{
    TArray<FString> Triggers;

    FRegexPattern ActionPattern(TEXT("\\{([^}]+)\\}"));
    FRegexMatcher ActionMatcher(ActionPattern, Response);

    while (ActionMatcher.FindNext())
    {
        FString Action = ActionMatcher.GetCaptureGroup(1);
        Triggers.Add(Action);
    }

    return Triggers;
}

void UAIConversationManager::SetPersonality(const FCharacterPersonality& NewPersonality)
{
    Personality = NewPersonality;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Personality updated to: %s"), *Personality.Name);
}

FConversationMemory UAIConversationManager::GetUserMemory(const FString& UserID) const
{
    if (const FConversationMemory* Memory = UserMemories.Find(UserID))
    {
        return *Memory;
    }

    // Return default memory for new users
    FConversationMemory NewMemory;
    NewMemory.UserID = UserID;
    NewMemory.FirstInteraction = FDateTime::Now();
    NewMemory.LastInteraction = FDateTime::Now();
    return NewMemory;
}

void UAIConversationManager::UpdateUserMemory(const FString& UserID, const FConversationMemory& Memory)
{
    UserMemories.Add(UserID, Memory);
}

void UAIConversationManager::ClearConversationHistory()
{
    ConversationHistory.Empty();
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Conversation history cleared"));
}

void UAIConversationManager::SetAPIEndpoint(const FString& Endpoint, const FString& APIKey)
{
    LLMEndpoint = Endpoint;
    LLMAPIKey = APIKey;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("LLM API endpoint configured: %s"), *Endpoint);
}

void UAIConversationManager::AddToHistory(const FString& Role, const FString& Content)
{
    ConversationHistory.Add(TPair<FString, FString>(Role, Content));

    // Trim history if too long
    while (ConversationHistory.Num() > MaxHistoryLength)
    {
        ConversationHistory.RemoveAt(0);
    }
}

FString UAIConversationManager::BuildContextWindow() const
{
    FString Context;

    // Include last N messages for context
    int32 ContextStart = FMath::Max(0, ConversationHistory.Num() - 10);
    for (int32 i = ContextStart; i < ConversationHistory.Num(); i++)
    {
        const auto& Entry = ConversationHistory[i];
        Context += FString::Printf(TEXT("%s: %s\n"), *Entry.Key, *Entry.Value);
    }

    return Context;
}
