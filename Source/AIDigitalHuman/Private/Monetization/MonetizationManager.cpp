// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Monetization/MonetizationManager.h"
#include "AIDigitalHumanModule.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMonetizationManager::UMonetizationManager()
{
}

void UMonetizationManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing Monetization Manager..."));

    // Setup default tip reactions
    FTipReaction SmallTip;
    SmallTip.MinimumAmount = 1.0f;
    SmallTip.AnimationName = TEXT("ThankYou");
    SmallTip.Emotion = EDigitalHumanEmotion::Happy;
    SmallTip.EmotionIntensity = 0.5f;
    SmallTip.Duration = 2.0f;
    TipReactions.Add(SmallTip);

    FTipReaction MediumTip;
    MediumTip.MinimumAmount = 10.0f;
    MediumTip.AnimationName = TEXT("Grateful");
    MediumTip.Emotion = EDigitalHumanEmotion::Happy;
    MediumTip.EmotionIntensity = 0.7f;
    MediumTip.CameraShot = ECameraShotType::MediumCloseUp;
    MediumTip.Duration = 3.0f;
    TipReactions.Add(MediumTip);

    FTipReaction LargeTip;
    LargeTip.MinimumAmount = 50.0f;
    LargeTip.AnimationName = TEXT("Excited");
    LargeTip.Emotion = EDigitalHumanEmotion::Excited;
    LargeTip.EmotionIntensity = 0.9f;
    LargeTip.CameraShot = ECameraShotType::CloseUp;
    LargeTip.Duration = 4.0f;
    TipReactions.Add(LargeTip);

    FTipReaction MassiveTip;
    MassiveTip.MinimumAmount = 100.0f;
    MassiveTip.AnimationName = TEXT("Celebration");
    MassiveTip.Emotion = EDigitalHumanEmotion::Excited;
    MassiveTip.EmotionIntensity = 1.0f;
    MassiveTip.VoiceLine = TEXT("Oh my goodness, thank you so much!");
    MassiveTip.CameraShot = ECameraShotType::CloseUp;
    MassiveTip.Duration = 5.0f;
    TipReactions.Add(MassiveTip);

    // Sort by amount (descending) for proper lookup
    TipReactions.Sort([](const FTipReaction& A, const FTipReaction& B)
    {
        return A.MinimumAmount > B.MinimumAmount;
    });

    // Setup default subscription benefits
    FSubscriptionBenefits FreeBenefits;
    FreeBenefits.Tier = EUserTier::Free;
    FreeBenefits.QueuePriorityMultiplier = 1.0f;
    SubscriptionBenefitsMap.Add(EUserTier::Free, FreeBenefits);

    FSubscriptionBenefits SubBenefits;
    SubBenefits.Tier = EUserTier::Subscriber;
    SubBenefits.QueuePriorityMultiplier = 1.5f;
    SubBenefits.ExclusiveAnimations = { TEXT("SubDance"), TEXT("SubWave") };
    SubBenefits.ExclusiveCommands = { TEXT("!dance") };
    SubscriptionBenefitsMap.Add(EUserTier::Subscriber, SubBenefits);

    FSubscriptionBenefits PremiumBenefits;
    PremiumBenefits.Tier = EUserTier::Premium;
    PremiumBenefits.QueuePriorityMultiplier = 2.0f;
    PremiumBenefits.ExclusiveAnimations = { TEXT("PremiumPose"), TEXT("PremiumDance") };
    PremiumBenefits.ExclusiveCommands = { TEXT("!pose"), TEXT("!dance") };
    PremiumBenefits.bPersonalGreeting = true;
    SubscriptionBenefitsMap.Add(EUserTier::Premium, PremiumBenefits);

    FSubscriptionBenefits VIPBenefits;
    VIPBenefits.Tier = EUserTier::VIP;
    VIPBenefits.QueuePriorityMultiplier = 3.0f;
    VIPBenefits.ExclusiveAnimations = { TEXT("VIPSpecial"), TEXT("VIPDance"), TEXT("VIPPose") };
    VIPBenefits.ExclusiveCommands = { TEXT("!vip"), TEXT("!pose"), TEXT("!dance") };
    VIPBenefits.bPersonalGreeting = true;
    VIPBenefits.bCustomNickname = true;
    SubscriptionBenefitsMap.Add(EUserTier::VIP, VIPBenefits);

    FSubscriptionBenefits WhaleBenefits;
    WhaleBenefits.Tier = EUserTier::Whale;
    WhaleBenefits.QueuePriorityMultiplier = 5.0f;
    WhaleBenefits.ExclusiveAnimations = { TEXT("WhaleSpecial"), TEXT("AllAnimations") };
    WhaleBenefits.ExclusiveCommands = { TEXT("!all") };
    WhaleBenefits.bPersonalGreeting = true;
    WhaleBenefits.bCustomNickname = true;
    WhaleBenefits.bDirectMessages = true;
    SubscriptionBenefitsMap.Add(EUserTier::Whale, WhaleBenefits);

    // Start cleanup timer for expired states
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CleanupTimer,
            this,
            &UMonetizationManager::CleanupExpiredStates,
            60.0f, // Check every minute
            true
        );
    }

    bIsInitialized = true;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Monetization Manager Initialized"));
}

void UMonetizationManager::OnTip(float Amount, const FString& Username, const FString& Message)
{
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Tip received: $%.2f from %s"), Amount, *Username);

    // Create event
    FMonetizationEvent Event;
    Event.EventType = TEXT("Tip");
    Event.Username = Username;
    Event.Amount = Amount;
    Event.Message = Message;
    Event.Timestamp = FDateTime::Now();

    // Update analytics
    SessionTotalTips += Amount;
    SessionTipCount++;

    // Update user spending
    float CurrentSpending = UserSpending.FindRef(Username);
    UserSpending.Add(Username, CurrentSpending + Amount);

    // Update user tier
    UpdateUserTier(Username, CurrentSpending + Amount);

    // Get and trigger reaction
    FTipReaction Reaction = GetTipReaction(Amount);
    OnReactionTriggered.Broadcast(Reaction);

    // Broadcast events
    OnTipReceived.Broadcast(Amount, Username);
    OnEvent.Broadcast(Event);
}

void UMonetizationManager::OnPaidMessage(const FString& Text, float Amount, const FString& Username)
{
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Paid message: $%.2f from %s - %s"), Amount, *Username, *Text);

    // Create event
    FMonetizationEvent Event;
    Event.EventType = TEXT("PaidMessage");
    Event.Username = Username;
    Event.Amount = Amount;
    Event.Message = Text;
    Event.Timestamp = FDateTime::Now();

    // Update analytics
    SessionTotalTips += Amount;
    SessionTipCount++;

    // Update user spending
    float CurrentSpending = UserSpending.FindRef(Username);
    UserSpending.Add(Username, CurrentSpending + Amount);

    // Trigger tip reaction as well
    FTipReaction Reaction = GetTipReaction(Amount);
    OnReactionTriggered.Broadcast(Reaction);

    OnEvent.Broadcast(Event);
}

void UMonetizationManager::OnSubscription(EUserTier Level, const FString& Username)
{
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Subscription: %s subscribed at tier %d"), *Username, static_cast<int32>(Level));

    // Create event
    FMonetizationEvent Event;
    Event.EventType = TEXT("Subscription");
    Event.Username = Username;
    Event.NewTier = Level;
    Event.Timestamp = FDateTime::Now();

    // Update analytics
    SessionSubscriptionCount++;

    // Update user tier
    UserTiers.Add(Username, Level);

    // Broadcast events
    OnSubscriptionReceived.Broadcast(Level, Username);
    OnEvent.Broadcast(Event);
}

void UMonetizationManager::ProcessEvent(const FMonetizationEvent& Event)
{
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Processing monetization event: %s from %s"),
        *Event.EventType, *Event.Username);

    if (Event.EventType == TEXT("Tip"))
    {
        OnTip(Event.Amount, Event.Username, Event.Message);
    }
    else if (Event.EventType == TEXT("PaidMessage"))
    {
        OnPaidMessage(Event.Message, Event.Amount, Event.Username);
    }
    else if (Event.EventType == TEXT("Subscription"))
    {
        OnSubscription(Event.NewTier, Event.Username);
    }
    else
    {
        // Generic event handling
        OnEvent.Broadcast(Event);
    }
}

FTipReaction UMonetizationManager::GetTipReaction(float Amount) const
{
    // TipReactions is sorted by MinimumAmount descending
    for (const FTipReaction& Reaction : TipReactions)
    {
        if (Amount >= Reaction.MinimumAmount)
        {
            return Reaction;
        }
    }

    // Default reaction for small tips
    FTipReaction DefaultReaction;
    DefaultReaction.AnimationName = TEXT("Nod");
    DefaultReaction.Emotion = EDigitalHumanEmotion::Happy;
    DefaultReaction.EmotionIntensity = 0.3f;
    DefaultReaction.Duration = 1.0f;
    return DefaultReaction;
}

void UMonetizationManager::AddTipReaction(const FTipReaction& Reaction)
{
    TipReactions.Add(Reaction);

    // Re-sort
    TipReactions.Sort([](const FTipReaction& A, const FTipReaction& B)
    {
        return A.MinimumAmount > B.MinimumAmount;
    });

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Added tip reaction for $%.2f+"), Reaction.MinimumAmount);
}

void UMonetizationManager::SetSubscriptionBenefits(const FSubscriptionBenefits& Benefits)
{
    SubscriptionBenefitsMap.Add(Benefits.Tier, Benefits);
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Set subscription benefits for tier %d"), static_cast<int32>(Benefits.Tier));
}

FSubscriptionBenefits UMonetizationManager::GetSubscriptionBenefits(EUserTier Tier) const
{
    if (const FSubscriptionBenefits* Benefits = SubscriptionBenefitsMap.Find(Tier))
    {
        return *Benefits;
    }

    // Return default
    FSubscriptionBenefits Default;
    Default.Tier = Tier;
    return Default;
}

void UMonetizationManager::RegisterInteractionState(const FString& StateName, float UnlockCost)
{
    FInteractionState State;
    State.StateName = StateName;
    State.UnlockCost = UnlockCost;
    State.bIsUnlocked = false;

    InteractionStates.Add(StateName, State);
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Registered interaction state: %s (cost: $%.2f)"), *StateName, UnlockCost);
}

void UMonetizationManager::UnlockInteractionState(const FString& StateName, float Duration)
{
    FInteractionState* State = InteractionStates.Find(StateName);
    if (!State)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Interaction state not found: %s"), *StateName);
        return;
    }

    State->bIsUnlocked = true;

    if (Duration > 0.0f)
    {
        State->UnlockedUntil = FDateTime::Now() + FTimespan::FromSeconds(Duration);
    }
    else
    {
        State->UnlockedUntil = FDateTime::MaxValue();
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Unlocked interaction state: %s"), *StateName);
    OnInteractionStateChanged.Broadcast(StateName, true);
}

void UMonetizationManager::LockInteractionState(const FString& StateName)
{
    FInteractionState* State = InteractionStates.Find(StateName);
    if (!State)
    {
        return;
    }

    State->bIsUnlocked = false;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Locked interaction state: %s"), *StateName);
    OnInteractionStateChanged.Broadcast(StateName, false);
}

bool UMonetizationManager::IsInteractionStateUnlocked(const FString& StateName) const
{
    const FInteractionState* State = InteractionStates.Find(StateName);
    if (!State)
    {
        return false;
    }

    if (!State->bIsUnlocked)
    {
        return false;
    }

    // Check expiration
    if (State->UnlockedUntil < FDateTime::Now())
    {
        return false;
    }

    return true;
}

TArray<FInteractionState> UMonetizationManager::GetAllInteractionStates() const
{
    TArray<FInteractionState> States;
    InteractionStates.GenerateValueArray(States);
    return States;
}

void UMonetizationManager::UpdateUserTier(const FString& UserID, float TotalSpent)
{
    EUserTier NewTier = CalculateTierFromSpending(TotalSpent);
    EUserTier CurrentTier = UserTiers.FindRef(UserID);

    if (NewTier > CurrentTier)
    {
        UserTiers.Add(UserID, NewTier);
        UE_LOG(LogAIDigitalHuman, Log, TEXT("User %s promoted to tier %d"), *UserID, static_cast<int32>(NewTier));
    }
}

EUserTier UMonetizationManager::GetUserTier(const FString& UserID) const
{
    return UserTiers.FindRef(UserID);
}

float UMonetizationManager::GetUserTotalSpending(const FString& UserID) const
{
    return UserSpending.FindRef(UserID);
}

float UMonetizationManager::GetUserTotalTips(const FString& UserID) const
{
    return UserSpending.FindRef(UserID);
}

TArray<FString> UMonetizationManager::GetTopTippers(int32 Count) const
{
    // Sort users by spending
    TArray<TPair<FString, float>> SortedUsers;
    for (const auto& Pair : UserSpending)
    {
        SortedUsers.Add(TPair<FString, float>(Pair.Key, Pair.Value));
    }

    SortedUsers.Sort([](const TPair<FString, float>& A, const TPair<FString, float>& B)
    {
        return A.Value > B.Value;
    });

    // Return top N
    TArray<FString> TopTippers;
    int32 ResultCount = FMath::Min(Count, SortedUsers.Num());
    for (int32 i = 0; i < ResultCount; i++)
    {
        TopTippers.Add(SortedUsers[i].Key);
    }

    return TopTippers;
}

EUserTier UMonetizationManager::CalculateTierFromSpending(float TotalSpent) const
{
    if (TotalSpent >= WhaleThreshold)
    {
        return EUserTier::Whale;
    }
    if (TotalSpent >= VIPThreshold)
    {
        return EUserTier::VIP;
    }
    if (TotalSpent >= PremiumThreshold)
    {
        return EUserTier::Premium;
    }
    if (TotalSpent >= SubscriberThreshold)
    {
        return EUserTier::Subscriber;
    }
    return EUserTier::Free;
}

void UMonetizationManager::CleanupExpiredStates()
{
    FDateTime Now = FDateTime::Now();

    for (auto& Pair : InteractionStates)
    {
        if (Pair.Value.bIsUnlocked && Pair.Value.UnlockedUntil < Now)
        {
            Pair.Value.bIsUnlocked = false;
            OnInteractionStateChanged.Broadcast(Pair.Key, false);
            UE_LOG(LogAIDigitalHuman, Log, TEXT("Interaction state expired: %s"), *Pair.Key);
        }
    }
}
