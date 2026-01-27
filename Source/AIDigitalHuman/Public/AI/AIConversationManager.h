// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "AIConversationManager.generated.h"

/**
 * Conversation memory entry for long-term context
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FConversationMemory
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    FString UserID;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    FString Username;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    TArray<FString> PreviousInteractions;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    TMap<FString, FString> UserPreferences;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    float RelationshipScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    EUserTier UserTier = EUserTier::Free;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    FDateTime FirstInteraction;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    FDateTime LastInteraction;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    int32 TotalInteractions = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Memory")
    float TotalSpent = 0.0f;
};

/**
 * Character personality configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FCharacterPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    FString Name = TEXT("Nova");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    FString Description = TEXT("A warm, engaging digital companion");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (MultiLine = true))
    FString SystemPrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    TArray<FString> PersonalityTraits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    TArray<FString> Interests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    TArray<FString> SpeakingStyle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    float Warmth = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    float Playfulness = 0.6f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    float Intelligence = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    float Confidence = 0.7f;
};

/**
 * Pending conversation request in the queue
 */
USTRUCT()
struct FPendingConversation
{
    GENERATED_BODY()

    FChatMessage Message;
    EChatPriority Priority = EChatPriority::Normal;
    FDateTime QueuedTime;
    bool bProcessing = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConversationResponseReady, const FAIResponse&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConversationError, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConversationThinking);

/**
 * Manages AI conversation, LLM integration, and dialogue generation.
 * Handles long-term memory, personality consistency, and natural responses.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UAIConversationManager : public UObject
{
    GENERATED_BODY()

public:
    UAIConversationManager();

    /** Initialize the conversation manager */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    void Initialize();

    /** Process an incoming chat message */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    void ProcessMessage(const FChatMessage& Message);

    /** Generate a response to a specific prompt */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    void GenerateResponse(const FString& Prompt, EDigitalHumanEmotion ContextEmotion = EDigitalHumanEmotion::Neutral);

    /** Set the character personality */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    void SetPersonality(const FCharacterPersonality& NewPersonality);

    /** Get the current personality */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    FCharacterPersonality GetPersonality() const { return Personality; }

    /** Get memory for a specific user */
    UFUNCTION(BlueprintCallable, Category = "Conversation|Memory")
    FConversationMemory GetUserMemory(const FString& UserID) const;

    /** Update user memory */
    UFUNCTION(BlueprintCallable, Category = "Conversation|Memory")
    void UpdateUserMemory(const FString& UserID, const FConversationMemory& Memory);

    /** Clear conversation history (keeps long-term memory) */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    void ClearConversationHistory();

    /** Set the LLM API endpoint */
    UFUNCTION(BlueprintCallable, Category = "Conversation|Config")
    void SetAPIEndpoint(const FString& Endpoint, const FString& APIKey);

    /** Check if currently processing a response */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    bool IsProcessing() const { return bIsProcessing; }

    /** Get the current queue size */
    UFUNCTION(BlueprintCallable, Category = "Conversation")
    int32 GetQueueSize() const { return ConversationQueue.Num(); }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Conversation|Events")
    FOnConversationResponseReady OnResponseReady;

    UPROPERTY(BlueprintAssignable, Category = "Conversation|Events")
    FOnConversationError OnError;

    UPROPERTY(BlueprintAssignable, Category = "Conversation|Events")
    FOnConversationThinking OnThinking;

protected:
    /** Process the next item in the queue */
    void ProcessQueue();

    /** Build the full prompt with context and memory */
    FString BuildPrompt(const FChatMessage& Message);

    /** Parse LLM response into structured data */
    FAIResponse ParseLLMResponse(const FString& RawResponse);

    /** Analyze emotion from text */
    EDigitalHumanEmotion AnalyzeEmotion(const FString& Text);

    /** Extract animation triggers from response */
    TArray<FString> ExtractAnimationTriggers(const FString& Response);

    /** Handle HTTP response from LLM API */
    void OnLLMResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

    /** Add message to conversation history */
    void AddToHistory(const FString& Role, const FString& Content);

    /** Build context window from history */
    FString BuildContextWindow() const;

private:
    UPROPERTY()
    FCharacterPersonality Personality;

    // Conversation history for current session
    TArray<TPair<FString, FString>> ConversationHistory;

    // Long-term memory storage (UserID -> Memory)
    TMap<FString, FConversationMemory> UserMemories;

    // Priority queue for incoming messages
    TArray<FPendingConversation> ConversationQueue;

    // API Configuration
    FString LLMEndpoint;
    FString LLMAPIKey;
    FString LLMModel = TEXT("gpt-4");

    // State
    bool bIsProcessing = false;
    bool bIsInitialized = false;

    // Limits
    int32 MaxHistoryLength = 50;
    int32 MaxQueueSize = 100;
    float QueueTimeoutSeconds = 30.0f;

    // Current processing context
    FChatMessage CurrentMessage;
};
