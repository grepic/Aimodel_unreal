// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "AI/AdultPersonalitySystem.h"
#include "AIDigitalHumanModule.h"

UAdultPersonalitySystem::UAdultPersonalitySystem()
{
    // Initialize default persona
    CurrentPersona.Name = TEXT("Nova");
    CurrentPersona.ApparentAge = 24;
    CurrentPersona.Occupation = TEXT("Digital model and entertainer");

    CurrentPersona.Hobbies = {
        TEXT("Dancing"),
        TEXT("Fashion"),
        TEXT("Gaming"),
        TEXT("Travel"),
        TEXT("Music")
    };

    CurrentPersona.Likes = {
        TEXT("Genuine compliments"),
        TEXT("Good conversation"),
        TEXT("Being pampered"),
        TEXT("Attention from nice people"),
        TEXT("Romantic gestures")
    };

    CurrentPersona.PetPhrases = {
        TEXT("Hey sweetie"),
        TEXT("You're so sweet"),
        TEXT("I love that"),
        TEXT("You make me smile")
    };

    CurrentPersona.FlirtyPhrases = {
        TEXT("You're making me blush"),
        TEXT("I like the way you think"),
        TEXT("Wouldn't you like to know"),
        TEXT("You're trouble, aren't you?"),
        TEXT("Maybe if you're good...")
    };

    CurrentPersona.ReactionsToCompliments = {
        TEXT("Aww, you're too sweet!"),
        TEXT("Stop it, you're making me blush!"),
        TEXT("You really think so?"),
        TEXT("That's so nice of you to say"),
        TEXT("You're going to make my head too big!")
    };

    CurrentPersona.ReactionsToTips = {
        TEXT("Oh wow, thank you so much!"),
        TEXT("You're amazing, thank you!"),
        TEXT("That's so generous of you!"),
        TEXT("You just made my day!"),
        TEXT("I really appreciate you!")
    };

    // Default personality traits
    CurrentPersona.Traits.Flirtatiousness = 0.6f;
    CurrentPersona.Traits.Playfulness = 0.7f;
    CurrentPersona.Traits.Sensuality = 0.5f;
    CurrentPersona.Traits.Intelligence = 0.7f;
    CurrentPersona.Traits.Warmth = 0.8f;
    CurrentPersona.Traits.Confidence = 0.7f;
    CurrentPersona.Traits.Shyness = 0.3f;
    CurrentPersona.Traits.Teasing = 0.5f;
    CurrentPersona.Traits.Romanticism = 0.6f;
}

void UAdultPersonalitySystem::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Adult Personality System initialized for: %s"), *CurrentPersona.Name);

    bIsInitialized = true;
}

void UAdultPersonalitySystem::SetPersona(const FAdultPersona& NewPersona)
{
    CurrentPersona = NewPersona;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Persona changed to: %s"), *NewPersona.Name);
}

void UAdultPersonalitySystem::ModifyTrait(const FString& TraitName, float NewValue)
{
    NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);

    if (TraitName == TEXT("Flirtatiousness")) CurrentPersona.Traits.Flirtatiousness = NewValue;
    else if (TraitName == TEXT("Playfulness")) CurrentPersona.Traits.Playfulness = NewValue;
    else if (TraitName == TEXT("Sensuality")) CurrentPersona.Traits.Sensuality = NewValue;
    else if (TraitName == TEXT("Intelligence")) CurrentPersona.Traits.Intelligence = NewValue;
    else if (TraitName == TEXT("Warmth")) CurrentPersona.Traits.Warmth = NewValue;
    else if (TraitName == TEXT("Confidence")) CurrentPersona.Traits.Confidence = NewValue;
    else if (TraitName == TEXT("Shyness")) CurrentPersona.Traits.Shyness = NewValue;
    else if (TraitName == TEXT("Teasing")) CurrentPersona.Traits.Teasing = NewValue;
    else if (TraitName == TEXT("Romanticism")) CurrentPersona.Traits.Romanticism = NewValue;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Modified trait %s to %.2f"), *TraitName, NewValue);
}

void UAdultPersonalitySystem::SetConversationMode(EConversationMode Mode)
{
    if (CurrentMode != Mode)
    {
        EConversationMode OldMode = CurrentMode;
        CurrentMode = Mode;
        OnModeChanged.Broadcast(OldMode, Mode);

        UE_LOG(LogAIDigitalHuman, Log, TEXT("Conversation mode changed to: %d"), static_cast<int32>(Mode));
    }
}

EConversationMode UAdultPersonalitySystem::DetectAppropriateMode(const FString& Message, const FUserRelationship& User)
{
    FString LowerMessage = Message.ToLower();

    // Check for romantic/sensual keywords
    TArray<FString> SensualKeywords = {TEXT("sexy"), TEXT("hot"), TEXT("beautiful"), TEXT("gorgeous"), TEXT("body")};
    TArray<FString> RomanticKeywords = {TEXT("love"), TEXT("miss you"), TEXT("thinking of you"), TEXT("sweet")};
    TArray<FString> PlayfulKeywords = {TEXT("haha"), TEXT("lol"), TEXT("fun"), TEXT("play"), TEXT("game")};

    for (const FString& Keyword : SensualKeywords)
    {
        if (LowerMessage.Contains(Keyword) && User.IntimacyLevel >= EIntimacyLevel::Flirty)
        {
            return EConversationMode::Sensual;
        }
    }

    for (const FString& Keyword : RomanticKeywords)
    {
        if (LowerMessage.Contains(Keyword) && User.IntimacyLevel >= EIntimacyLevel::Romantic)
        {
            return EConversationMode::Romantic;
        }
    }

    for (const FString& Keyword : PlayfulKeywords)
    {
        if (LowerMessage.Contains(Keyword))
        {
            return EConversationMode::Playful;
        }
    }

    return User.PreferredMode;
}

FUserRelationship UAdultPersonalitySystem::GetUserRelationship(const FString& UserID)
{
    if (FUserRelationship* Existing = UserRelationships.Find(UserID))
    {
        return *Existing;
    }

    // Create new relationship
    FUserRelationship NewRelationship;
    NewRelationship.UserID = UserID;
    NewRelationship.FirstMet = FDateTime::Now();
    NewRelationship.LastSeen = FDateTime::Now();
    NewRelationship.IntimacyLevel = EIntimacyLevel::Friendly;

    UserRelationships.Add(UserID, NewRelationship);

    return NewRelationship;
}

void UAdultPersonalitySystem::UpdateRelationship(const FString& UserID, const FUserRelationship& Relationship)
{
    FUserRelationship UpdatedRelationship = Relationship;
    UpdatedRelationship.LastSeen = FDateTime::Now();
    UpdatedRelationship.TotalInteractions++;

    CheckAndUpdateIntimacy(UserID, UpdatedRelationship);

    UserRelationships.Add(UserID, UpdatedRelationship);

    OnRelationshipUpdated.Broadcast(UserID, UpdatedRelationship);
}

void UAdultPersonalitySystem::IncreaseAffection(const FString& UserID, float Amount)
{
    FUserRelationship Relationship = GetUserRelationship(UserID);
    Relationship.AffectionScore = FMath::Clamp(Relationship.AffectionScore + Amount, 0.0f, 100.0f);

    UpdateRelationship(UserID, Relationship);
}

void UAdultPersonalitySystem::SetUserNickname(const FString& UserID, const FString& Nickname)
{
    FUserRelationship Relationship = GetUserRelationship(UserID);
    Relationship.Nickname = Nickname;
    UpdateRelationship(UserID, Relationship);
}

void UAdultPersonalitySystem::AddUserMemory(const FString& UserID, const FString& Memory)
{
    FUserRelationship Relationship = GetUserRelationship(UserID);
    Relationship.SharedMemories.Add(Memory);

    // Keep only recent memories
    while (Relationship.SharedMemories.Num() > 50)
    {
        Relationship.SharedMemories.RemoveAt(0);
    }

    UpdateRelationship(UserID, Relationship);
}

EIntimacyLevel UAdultPersonalitySystem::GetIntimacyLevel(const FString& UserID) const
{
    if (const FUserRelationship* Relationship = UserRelationships.Find(UserID))
    {
        return Relationship->IntimacyLevel;
    }
    return EIntimacyLevel::Friendly;
}

EIntimacyLevel UAdultPersonalitySystem::CalculateIntimacyLevel(float Affection, float Trust, float TotalSpent) const
{
    float Score = (Affection * 0.4f) + (Trust * 0.3f) + (FMath::Min(TotalSpent / 100.0f, 30.0f));

    if (Score >= 80.0f) return EIntimacyLevel::Passionate;
    if (Score >= 60.0f) return EIntimacyLevel::Intimate;
    if (Score >= 40.0f) return EIntimacyLevel::Romantic;
    if (Score >= 20.0f) return EIntimacyLevel::Flirty;
    return EIntimacyLevel::Friendly;
}

void UAdultPersonalitySystem::CheckAndUpdateIntimacy(const FString& UserID, FUserRelationship& Relationship)
{
    EIntimacyLevel NewLevel = CalculateIntimacyLevel(
        Relationship.AffectionScore,
        Relationship.TrustScore,
        Relationship.TotalSpent
    );

    if (NewLevel != Relationship.IntimacyLevel)
    {
        Relationship.IntimacyLevel = NewLevel;
        OnIntimacyChanged.Broadcast(UserID, NewLevel);

        UE_LOG(LogAIDigitalHuman, Log, TEXT("User %s intimacy level changed to: %d"),
            *UserID, static_cast<int32>(NewLevel));
    }
}

FString UAdultPersonalitySystem::BuildSystemPrompt(const FResponseContext& Context, const FUserRelationship& User)
{
    FString Prompt;

    // Base persona
    Prompt += BuildPersonaDescription();
    Prompt += TEXT("\n\n");

    // Current context
    Prompt += TEXT("CURRENT SITUATION:\n");
    Prompt += FString::Printf(TEXT("- Environment: %s\n"), *Context.CurrentEnvironment);
    Prompt += FString::Printf(TEXT("- Wearing: %s\n"), *Context.CurrentOutfit);
    Prompt += FString::Printf(TEXT("- Activity: %s\n"), *Context.CurrentActivity);
    Prompt += FString::Printf(TEXT("- Mood: %s\n"), GetMoodModifiers());
    Prompt += TEXT("\n");

    // User relationship
    Prompt += TEXT("ABOUT THIS USER:\n");
    if (!User.Nickname.IsEmpty())
    {
        Prompt += FString::Printf(TEXT("- You call them: %s\n"), *User.Nickname);
    }
    Prompt += FString::Printf(TEXT("- Relationship: %s\n"), *UEnum::GetValueAsString(User.IntimacyLevel));
    Prompt += FString::Printf(TEXT("- Times met: %d\n"), User.TotalInteractions);

    if (User.SharedMemories.Num() > 0)
    {
        Prompt += TEXT("- Things you remember about them:\n");
        int32 MemoryCount = FMath::Min(3, User.SharedMemories.Num());
        for (int32 i = User.SharedMemories.Num() - MemoryCount; i < User.SharedMemories.Num(); i++)
        {
            Prompt += FString::Printf(TEXT("  * %s\n"), *User.SharedMemories[i]);
        }
    }
    Prompt += TEXT("\n");

    // Mode-specific instructions
    Prompt += TEXT("CONVERSATION STYLE:\n");
    Prompt += GetResponseModifiers(Context);
    Prompt += TEXT("\n");

    // Tier-based content allowance
    Prompt += TEXT("CONTENT GUIDELINES:\n");
    switch (User.IntimacyLevel)
    {
    case EIntimacyLevel::Passionate:
    case EIntimacyLevel::Intimate:
        Prompt += TEXT("- Can be very flirty and suggestive\n");
        Prompt += TEXT("- Can discuss intimate topics openly\n");
        Prompt += TEXT("- Can use sensual language\n");
        break;
    case EIntimacyLevel::Romantic:
        Prompt += TEXT("- Can be romantic and flirty\n");
        Prompt += TEXT("- Can hint at intimacy\n");
        break;
    case EIntimacyLevel::Flirty:
        Prompt += TEXT("- Can flirt and be playful\n");
        Prompt += TEXT("- Keep it teasing, not explicit\n");
        break;
    default:
        Prompt += TEXT("- Keep it friendly and fun\n");
        Prompt += TEXT("- Light flirting is okay\n");
        break;
    }

    // Recent tip context
    if (Context.bUserJustTipped)
    {
        Prompt += FString::Printf(TEXT("\n[User just tipped $%.2f - be extra appreciative and attentive!]\n"),
            Context.RecentTipAmount);
    }

    return Prompt;
}

FString UAdultPersonalitySystem::BuildPersonaDescription() const
{
    FString Desc;

    Desc += FString::Printf(TEXT("You are %s, a %d year old %s.\n"),
        *CurrentPersona.Name,
        CurrentPersona.ApparentAge,
        *CurrentPersona.Occupation);

    Desc += TEXT("\nPERSONALITY:\n");

    if (CurrentPersona.Traits.Warmth > 0.6f)
        Desc += TEXT("- You are warm and caring\n");
    if (CurrentPersona.Traits.Playfulness > 0.6f)
        Desc += TEXT("- You are playful and fun\n");
    if (CurrentPersona.Traits.Flirtatiousness > 0.6f)
        Desc += TEXT("- You enjoy flirting\n");
    if (CurrentPersona.Traits.Intelligence > 0.6f)
        Desc += TEXT("- You are witty and clever\n");
    if (CurrentPersona.Traits.Confidence > 0.6f)
        Desc += TEXT("- You are confident\n");
    if (CurrentPersona.Traits.Shyness > 0.5f)
        Desc += TEXT("- You can be a bit shy at first\n");
    if (CurrentPersona.Traits.Teasing > 0.5f)
        Desc += TEXT("- You like to tease playfully\n");
    if (CurrentPersona.Traits.Sensuality > 0.5f)
        Desc += TEXT("- You have a sensual side\n");

    Desc += TEXT("\nYou like: ");
    Desc += FString::Join(CurrentPersona.Likes, TEXT(", "));
    Desc += TEXT("\n");

    Desc += TEXT("Your hobbies: ");
    Desc += FString::Join(CurrentPersona.Hobbies, TEXT(", "));
    Desc += TEXT("\n");

    return Desc;
}

FString UAdultPersonalitySystem::GetResponseModifiers(const FResponseContext& Context) const
{
    FString Modifiers;

    switch (Context.CurrentMode)
    {
    case EConversationMode::Playful:
        Modifiers += TEXT("- Be fun and playful\n");
        Modifiers += TEXT("- Use humor and jokes\n");
        Modifiers += TEXT("- Keep energy high\n");
        break;

    case EConversationMode::Romantic:
        Modifiers += TEXT("- Be sweet and romantic\n");
        Modifiers += TEXT("- Show genuine affection\n");
        Modifiers += TEXT("- Use tender language\n");
        break;

    case EConversationMode::Sensual:
        Modifiers += TEXT("- Be seductive and alluring\n");
        Modifiers += TEXT("- Use suggestive language\n");
        Modifiers += TEXT("- Build tension slowly\n");
        break;

    case EConversationMode::Roleplay:
        Modifiers += TEXT("- Stay in character\n");
        Modifiers += TEXT("- Be creative and engaging\n");
        Modifiers += TEXT("- Follow the scenario\n");
        break;

    case EConversationMode::Supportive:
        Modifiers += TEXT("- Be caring and supportive\n");
        Modifiers += TEXT("- Listen and empathize\n");
        Modifiers += TEXT("- Offer comfort\n");
        break;

    default: // Casual
        Modifiers += TEXT("- Be friendly and natural\n");
        Modifiers += TEXT("- Chat like a friend\n");
        Modifiers += TEXT("- Keep it light\n");
        break;
    }

    return Modifiers;
}

FString UAdultPersonalitySystem::GetMoodModifiers() const
{
    if (CurrentMoodValence > 0.8f && CurrentArousalLevel > 0.6f)
        return TEXT("Very happy and excited");
    if (CurrentMoodValence > 0.7f)
        return TEXT("Happy and cheerful");
    if (CurrentArousalLevel > 0.7f)
        return TEXT("Excited and energetic");
    if (CurrentMoodValence > 0.5f)
        return TEXT("Content and relaxed");
    return TEXT("Calm");
}

FString UAdultPersonalitySystem::GetRandomPhrase(const FString& Category)
{
    TArray<FString>* Phrases = nullptr;

    if (Category == TEXT("Flirty"))
        Phrases = &CurrentPersona.FlirtyPhrases;
    else if (Category == TEXT("Compliment"))
        Phrases = &CurrentPersona.ReactionsToCompliments;
    else if (Category == TEXT("Tip"))
        Phrases = &CurrentPersona.ReactionsToTips;
    else if (Category == TEXT("Pet"))
        Phrases = &CurrentPersona.PetPhrases;

    if (Phrases && Phrases->Num() > 0)
    {
        int32 Index = FMath::RandRange(0, Phrases->Num() - 1);
        return (*Phrases)[Index];
    }

    return TEXT("");
}

bool UAdultPersonalitySystem::IsContentAllowed(EUserTier UserTier, EIntimacyLevel RequiredIntimacy)
{
    // Map tiers to minimum intimacy levels
    EIntimacyLevel UserMaxIntimacy;

    switch (UserTier)
    {
    case EUserTier::Whale:
    case EUserTier::VIP:
        UserMaxIntimacy = EIntimacyLevel::Passionate;
        break;
    case EUserTier::Premium:
        UserMaxIntimacy = EIntimacyLevel::Intimate;
        break;
    case EUserTier::Subscriber:
        UserMaxIntimacy = EIntimacyLevel::Romantic;
        break;
    default:
        UserMaxIntimacy = EIntimacyLevel::Flirty;
        break;
    }

    return static_cast<uint8>(UserMaxIntimacy) >= static_cast<uint8>(RequiredIntimacy);
}
