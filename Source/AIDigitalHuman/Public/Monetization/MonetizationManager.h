// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "MonetizationManager.generated.h"

/**
 * Tip reaction configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FTipReaction
{
    GENERATED_BODY()

    /** Minimum tip amount to trigger this reaction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction")
    float MinimumAmount = 0.0f;

    /** Animation to play */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction")
    FString AnimationName;

    /** Emotion to express */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction")
    EDigitalHumanEmotion Emotion = EDigitalHumanEmotion::Happy;

    /** Emotion intensity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction")
    float EmotionIntensity = 0.7f;

    /** Special voice line (optional) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction")
    FString VoiceLine;

    /** Camera shot for this reaction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction")
    ECameraShotType CameraShot = ECameraShotType::MediumCloseUp;

    /** Duration to hold the reaction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction")
    float Duration = 3.0f;
};

/**
 * Subscription tier benefits
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FSubscriptionBenefits
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benefits")
    EUserTier Tier = EUserTier::Free;

    /** Priority multiplier for chat queue */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benefits")
    float QueuePriorityMultiplier = 1.0f;

    /** Exclusive animations available */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benefits")
    TArray<FString> ExclusiveAnimations;

    /** Exclusive commands available */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benefits")
    TArray<FString> ExclusiveCommands;

    /** Greeting on join */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benefits")
    bool bPersonalGreeting = false;

    /** Custom nickname support */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benefits")
    bool bCustomNickname = false;

    /** Direct message capability */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benefits")
    bool bDirectMessages = false;
};

/**
 * Interaction state unlocked by monetization
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FInteractionState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "State")
    FString StateName;

    UPROPERTY(BlueprintReadWrite, Category = "State")
    bool bIsUnlocked = false;

    UPROPERTY(BlueprintReadWrite, Category = "State")
    float UnlockCost = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "State")
    FDateTime UnlockedUntil;

    UPROPERTY(BlueprintReadWrite, Category = "State")
    TMap<FString, FString> StateParameters;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonetizationEventReceived, const FMonetizationEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTipReceived, float, Amount, const FString&, Username);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubscriptionReceived, EUserTier, Tier, const FString&, Username);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionStateChanged, const FString&, StateName, bool, bUnlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReactionTriggered, const FTipReaction&, Reaction);

/**
 * Manages monetization events and their effects on character behavior.
 * Handles tips, subscriptions, and interaction state unlocks.
 * NOTE: No payment processing happens here - only receives events and triggers reactions.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UMonetizationManager : public UObject
{
    GENERATED_BODY()

public:
    UMonetizationManager();

    /** Initialize the monetization manager */
    UFUNCTION(BlueprintCallable, Category = "Monetization")
    void Initialize();

    // ==================== Event Handlers ====================

    /** Handle incoming tip event */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Events")
    void OnTip(float Amount, const FString& Username, const FString& Message = TEXT(""));

    /** Handle incoming paid message event */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Events")
    void OnPaidMessage(const FString& Text, float Amount, const FString& Username);

    /** Handle subscription event */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Events")
    void OnSubscription(EUserTier Level, const FString& Username);

    /** Handle generic monetization event */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Events")
    void ProcessEvent(const FMonetizationEvent& Event);

    // ==================== Configuration ====================

    /** Add a tip reaction tier */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Config")
    void AddTipReaction(const FTipReaction& Reaction);

    /** Set subscription benefits for a tier */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Config")
    void SetSubscriptionBenefits(const FSubscriptionBenefits& Benefits);

    /** Get benefits for a subscription tier */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Config")
    FSubscriptionBenefits GetSubscriptionBenefits(EUserTier Tier) const;

    // ==================== Interaction States ====================

    /** Register an interaction state */
    UFUNCTION(BlueprintCallable, Category = "Monetization|States")
    void RegisterInteractionState(const FString& StateName, float UnlockCost);

    /** Unlock an interaction state */
    UFUNCTION(BlueprintCallable, Category = "Monetization|States")
    void UnlockInteractionState(const FString& StateName, float Duration = -1.0f);

    /** Lock an interaction state */
    UFUNCTION(BlueprintCallable, Category = "Monetization|States")
    void LockInteractionState(const FString& StateName);

    /** Check if an interaction state is unlocked */
    UFUNCTION(BlueprintCallable, Category = "Monetization|States")
    bool IsInteractionStateUnlocked(const FString& StateName) const;

    /** Get all interaction states */
    UFUNCTION(BlueprintCallable, Category = "Monetization|States")
    TArray<FInteractionState> GetAllInteractionStates() const;

    // ==================== User Management ====================

    /** Update user tier based on spending */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Users")
    void UpdateUserTier(const FString& UserID, float TotalSpent);

    /** Get user's current tier */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Users")
    EUserTier GetUserTier(const FString& UserID) const;

    /** Get user's total spending */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Users")
    float GetUserTotalSpending(const FString& UserID) const;

    // ==================== Analytics ====================

    /** Get total tips in current session */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Analytics")
    float GetSessionTotalTips() const { return SessionTotalTips; }

    /** Get total tips for a user */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Analytics")
    float GetUserTotalTips(const FString& UserID) const;

    /** Get top tippers */
    UFUNCTION(BlueprintCallable, Category = "Monetization|Analytics")
    TArray<FString> GetTopTippers(int32 Count = 10) const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Monetization|Events")
    FOnMonetizationEventReceived OnEvent;

    UPROPERTY(BlueprintAssignable, Category = "Monetization|Events")
    FOnTipReceived OnTipReceived;

    UPROPERTY(BlueprintAssignable, Category = "Monetization|Events")
    FOnSubscriptionReceived OnSubscriptionReceived;

    UPROPERTY(BlueprintAssignable, Category = "Monetization|Events")
    FOnInteractionStateChanged OnInteractionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Monetization|Events")
    FOnReactionTriggered OnReactionTriggered;

protected:
    /** Get appropriate reaction for tip amount */
    FTipReaction GetTipReaction(float Amount) const;

    /** Clean up expired interaction states */
    void CleanupExpiredStates();

    /** Calculate tier from total spending */
    EUserTier CalculateTierFromSpending(float TotalSpent) const;

private:
    // Configuration
    TArray<FTipReaction> TipReactions;
    TMap<EUserTier, FSubscriptionBenefits> SubscriptionBenefitsMap;
    TMap<FString, FInteractionState> InteractionStates;

    // User data
    TMap<FString, EUserTier> UserTiers;
    TMap<FString, float> UserSpending;

    // Session analytics
    float SessionTotalTips = 0.0f;
    int32 SessionTipCount = 0;
    int32 SessionSubscriptionCount = 0;

    // Tier thresholds
    float SubscriberThreshold = 10.0f;
    float PremiumThreshold = 100.0f;
    float VIPThreshold = 500.0f;
    float WhaleThreshold = 1000.0f;

    // State
    bool bIsInitialized = false;

    // Cleanup timer
    FTimerHandle CleanupTimer;
};
