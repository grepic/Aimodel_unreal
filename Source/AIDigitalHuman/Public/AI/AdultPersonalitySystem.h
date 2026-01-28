// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "AdultPersonalitySystem.generated.h"

/**
 * Interaction intimacy level
 */
UENUM(BlueprintType)
enum class EIntimacyLevel : uint8
{
    Friendly        UMETA(DisplayName = "Friendly"),
    Flirty          UMETA(DisplayName = "Flirty"),
    Romantic        UMETA(DisplayName = "Romantic"),
    Intimate        UMETA(DisplayName = "Intimate"),
    Passionate      UMETA(DisplayName = "Passionate")
};

/**
 * Conversation mode
 */
UENUM(BlueprintType)
enum class EConversationMode : uint8
{
    Casual          UMETA(DisplayName = "Casual Chat"),
    Playful         UMETA(DisplayName = "Playful/Fun"),
    Romantic        UMETA(DisplayName = "Romantic"),
    Sensual         UMETA(DisplayName = "Sensual"),
    Roleplay        UMETA(DisplayName = "Roleplay"),
    Supportive      UMETA(DisplayName = "Supportive/Caring")
};

/**
 * User relationship data
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FUserRelationship
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    FString UserID;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    FString Username;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    FString Nickname;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    EIntimacyLevel IntimacyLevel = EIntimacyLevel::Friendly;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    float AffectionScore = 0.0f; // 0-100

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    float TrustScore = 0.0f; // 0-100

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    int32 TotalInteractions = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    float TotalSpent = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    TArray<FString> KnownPreferences;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    TArray<FString> SharedMemories;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    FDateTime FirstMet;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    FDateTime LastSeen;

    UPROPERTY(BlueprintReadWrite, Category = "Relationship")
    EConversationMode PreferredMode = EConversationMode::Casual;
};

/**
 * Personality trait configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FPersonalityTraits
{
    GENERATED_BODY()

    /** How openly flirtatious (0=reserved, 1=very forward) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Flirtatiousness = 0.6f;

    /** Playfulness in conversation */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Playfulness = 0.7f;

    /** How sensual/suggestive */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Sensuality = 0.5f;

    /** Intelligence/wit display */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Intelligence = 0.7f;

    /** Warmth and caring */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Warmth = 0.8f;

    /** Confidence level */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Confidence = 0.7f;

    /** Shyness (inverse of boldness) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Shyness = 0.3f;

    /** How teasing/playfully mean */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Teasing = 0.5f;

    /** Romanticism level */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Romanticism = 0.6f;

    /** Submissive vs Dominant spectrum (0.5 = balanced) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DominanceSpectrum = 0.4f;
};

/**
 * Adult character persona
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FAdultPersona
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    FString Name = TEXT("Nova");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    int32 ApparentAge = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    FString Occupation = TEXT("Model and streamer");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> Hobbies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> Likes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> Dislikes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    FString BackgroundStory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    FPersonalityTraits Traits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> SpeechPatterns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> PetPhrases;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> FlirtyPhrases;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> ReactionsToCompliments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persona")
    TArray<FString> ReactionsToTips;
};

/**
 * Response modifier based on context
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FResponseContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    EConversationMode CurrentMode = EConversationMode::Casual;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    EIntimacyLevel IntimacyLevel = EIntimacyLevel::Friendly;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    EUserTier UserTier = EUserTier::Free;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    bool bUserJustTipped = false;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    float RecentTipAmount = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    bool bIsPrivateSession = false;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    FString CurrentActivity;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    FString CurrentOutfit;

    UPROPERTY(BlueprintReadWrite, Category = "Context")
    FString CurrentEnvironment;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIntimacyChanged, const FString&, UserID, EIntimacyLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModeChanged, EConversationMode, OldMode, EConversationMode, NewMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipUpdated, const FString&, UserID, const FUserRelationship&, Relationship);

/**
 * Manages personality, relationships, and conversation tone for adult content.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UAdultPersonalitySystem : public UObject
{
    GENERATED_BODY()

public:
    UAdultPersonalitySystem();

    /** Initialize the personality system */
    UFUNCTION(BlueprintCallable, Category = "Personality")
    void Initialize();

    // ==================== Persona Management ====================

    /** Set the character persona */
    UFUNCTION(BlueprintCallable, Category = "Personality|Persona")
    void SetPersona(const FAdultPersona& NewPersona);

    /** Get current persona */
    UFUNCTION(BlueprintCallable, Category = "Personality|Persona")
    FAdultPersona GetPersona() const { return CurrentPersona; }

    /** Modify personality trait at runtime */
    UFUNCTION(BlueprintCallable, Category = "Personality|Persona")
    void ModifyTrait(const FString& TraitName, float NewValue);

    // ==================== Conversation Mode ====================

    /** Set conversation mode */
    UFUNCTION(BlueprintCallable, Category = "Personality|Mode")
    void SetConversationMode(EConversationMode Mode);

    /** Get current mode */
    UFUNCTION(BlueprintCallable, Category = "Personality|Mode")
    EConversationMode GetConversationMode() const { return CurrentMode; }

    /** Auto-detect appropriate mode from message */
    UFUNCTION(BlueprintCallable, Category = "Personality|Mode")
    EConversationMode DetectAppropriateMode(const FString& Message, const FUserRelationship& User);

    // ==================== Relationship Management ====================

    /** Get or create user relationship */
    UFUNCTION(BlueprintCallable, Category = "Personality|Relationship")
    FUserRelationship GetUserRelationship(const FString& UserID);

    /** Update user relationship */
    UFUNCTION(BlueprintCallable, Category = "Personality|Relationship")
    void UpdateRelationship(const FString& UserID, const FUserRelationship& Relationship);

    /** Increase affection for user */
    UFUNCTION(BlueprintCallable, Category = "Personality|Relationship")
    void IncreaseAffection(const FString& UserID, float Amount);

    /** Set nickname for user */
    UFUNCTION(BlueprintCallable, Category = "Personality|Relationship")
    void SetUserNickname(const FString& UserID, const FString& Nickname);

    /** Add memory about user */
    UFUNCTION(BlueprintCallable, Category = "Personality|Relationship")
    void AddUserMemory(const FString& UserID, const FString& Memory);

    /** Get intimacy level for user */
    UFUNCTION(BlueprintCallable, Category = "Personality|Relationship")
    EIntimacyLevel GetIntimacyLevel(const FString& UserID) const;

    /** Calculate intimacy from scores */
    UFUNCTION(BlueprintCallable, Category = "Personality|Relationship")
    EIntimacyLevel CalculateIntimacyLevel(float Affection, float Trust, float TotalSpent) const;

    // ==================== Response Generation ====================

    /** Build system prompt for LLM based on context */
    UFUNCTION(BlueprintCallable, Category = "Personality|Response")
    FString BuildSystemPrompt(const FResponseContext& Context, const FUserRelationship& User);

    /** Get appropriate response modifiers */
    UFUNCTION(BlueprintCallable, Category = "Personality|Response")
    FString GetResponseModifiers(const FResponseContext& Context);

    /** Get random phrase for situation */
    UFUNCTION(BlueprintCallable, Category = "Personality|Response")
    FString GetRandomPhrase(const FString& Category);

    /** Determine if content level is allowed for user */
    UFUNCTION(BlueprintCallable, Category = "Personality|Response")
    bool IsContentAllowed(EUserTier UserTier, EIntimacyLevel RequiredIntimacy);

    // ==================== Events ====================

    UPROPERTY(BlueprintAssignable, Category = "Personality|Events")
    FOnIntimacyChanged OnIntimacyChanged;

    UPROPERTY(BlueprintAssignable, Category = "Personality|Events")
    FOnModeChanged OnModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Personality|Events")
    FOnRelationshipUpdated OnRelationshipUpdated;

protected:
    /** Build persona description for prompt */
    FString BuildPersonaDescription() const;

    /** Get mood modifiers based on recent events */
    FString GetMoodModifiers() const;

    /** Update intimacy level if changed */
    void CheckAndUpdateIntimacy(const FString& UserID, FUserRelationship& Relationship);

private:
    UPROPERTY()
    FAdultPersona CurrentPersona;

    UPROPERTY()
    EConversationMode CurrentMode = EConversationMode::Casual;

    UPROPERTY()
    TMap<FString, FUserRelationship> UserRelationships;

    bool bIsInitialized = false;

    // Mood state
    float CurrentMoodValence = 0.7f; // 0=negative, 1=positive
    float CurrentArousalLevel = 0.3f; // 0=calm, 1=excited
};
