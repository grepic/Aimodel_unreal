// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Safety/SafetyManager.h"
#include "AIDigitalHumanModule.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Internationalization/Regex.h"

USafetyManager::USafetyManager()
{
    Configuration.bAdultContentPlatform = true;
    Configuration.bShowAIDisclosure = true;
    Configuration.bEnableLogging = true;
    Configuration.bEnableRateLimiting = true;
    Configuration.MaxMessagesPerMinute = 10;

    Configuration.ContentFilter.bEnabled = true;
    Configuration.ContentFilter.MaxMessageLength = 500;
    Configuration.ContentFilter.bBlockExcessiveCaps = true;
    Configuration.ContentFilter.CapsThreshold = 0.7f;
    Configuration.ContentFilter.bBlockRepeatedMessages = true;
}

void USafetyManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Initializing Safety Manager..."));

    // Setup cleanup timer for rate limiting data
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CleanupTimer,
            this,
            &USafetyManager::CleanupRateLimits,
            60.0f, // Cleanup every minute
            true
        );
    }

    bIsInitialized = true;

    // Log initialization
    LogInteraction(TEXT("System"), TEXT("SafetyManager"), TEXT("Safety Manager initialized"));

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Safety Manager Initialized"));
}

void USafetyManager::ActivateEmergencyStop()
{
    if (bEmergencyStopped)
    {
        return;
    }

    bEmergencyStopped = true;

    UE_LOG(LogAIDigitalHuman, Warning, TEXT("*** EMERGENCY STOP ACTIVATED ***"));

    LogInteraction(TEXT("Emergency"), TEXT("System"), TEXT("Emergency stop activated"));

    OnEmergencyStopActivated.Broadcast();
}

void USafetyManager::Resume()
{
    if (!bEmergencyStopped)
    {
        return;
    }

    bEmergencyStopped = false;

    UE_LOG(LogAIDigitalHuman, Log, TEXT("System resumed from emergency stop"));

    LogInteraction(TEXT("Emergency"), TEXT("System"), TEXT("System resumed"));

    OnSystemResumed.Broadcast();
}

bool USafetyManager::ValidateMessage(const FChatMessage& Message)
{
    // Check if user is banned
    if (IsUserBanned(Message.UserID))
    {
        LogBlockedAction(TEXT("Message"), Message.Username, Message.Message, TEXT("User is banned"));
        return false;
    }

    // Check rate limit
    if (!CheckRateLimit(Message.UserID))
    {
        LogBlockedAction(TEXT("Message"), Message.Username, Message.Message, TEXT("Rate limit exceeded"));
        return false;
    }

    // Validate content
    FString BlockReason;
    if (!ValidateContent(Message.Message, BlockReason))
    {
        LogBlockedAction(TEXT("Message"), Message.Username, Message.Message, BlockReason);
        OnMessageBlocked.Broadcast(Message.Username, BlockReason);
        return false;
    }

    return true;
}

bool USafetyManager::ValidateContent(const FString& Content, FString& OutReason)
{
    if (!Configuration.ContentFilter.bEnabled)
    {
        return true;
    }

    // Check length
    if (Content.Len() > Configuration.ContentFilter.MaxMessageLength)
    {
        OutReason = TEXT("Message too long");
        return false;
    }

    // Check blacklist
    FString MatchedWord;
    if (CheckBlacklist(Content, MatchedWord))
    {
        OutReason = TEXT("Contains blocked content");
        return false;
    }

    // Check excessive caps
    if (Configuration.ContentFilter.bBlockExcessiveCaps && CheckExcessiveCaps(Content))
    {
        OutReason = TEXT("Excessive caps");
        return false;
    }

    return true;
}

bool USafetyManager::CheckRateLimit(const FString& UserID)
{
    if (!Configuration.bEnableRateLimiting)
    {
        return true;
    }

    FDateTime Now = FDateTime::Now();
    FDateTime OneMinuteAgo = Now - FTimespan::FromMinutes(1);

    // Get user's message timestamps
    TArray<FDateTime>& Timestamps = UserMessageTimestamps.FindOrAdd(UserID);

    // Remove old timestamps
    Timestamps.RemoveAll([&OneMinuteAgo](const FDateTime& Time)
    {
        return Time < OneMinuteAgo;
    });

    // Check count
    if (Timestamps.Num() >= Configuration.MaxMessagesPerMinute)
    {
        OnUserRateLimited.Broadcast(UserID, Timestamps.Num());
        return false;
    }

    // Add current timestamp
    Timestamps.Add(Now);

    return true;
}

bool USafetyManager::CheckBlacklist(const FString& Content, FString& OutMatchedWord) const
{
    FString LowerContent = Content.ToLower();

    // Check word blacklist
    for (const FString& Word : Configuration.ContentFilter.BlacklistedWords)
    {
        if (LowerContent.Contains(Word.ToLower()))
        {
            OutMatchedWord = Word;
            return true;
        }
    }

    // Check pattern blacklist
    for (const FString& Pattern : Configuration.ContentFilter.BlacklistedPatterns)
    {
        FRegexPattern RegexPattern(Pattern);
        FRegexMatcher Matcher(RegexPattern, Content);

        if (Matcher.FindNext())
        {
            OutMatchedWord = Pattern;
            return true;
        }
    }

    return false;
}

bool USafetyManager::CheckExcessiveCaps(const FString& Content) const
{
    if (Content.Len() < 10)
    {
        return false; // Too short to matter
    }

    int32 UpperCount = 0;
    int32 LetterCount = 0;

    for (TCHAR Char : Content)
    {
        if (FChar::IsAlpha(Char))
        {
            LetterCount++;
            if (FChar::IsUpper(Char))
            {
                UpperCount++;
            }
        }
    }

    if (LetterCount == 0)
    {
        return false;
    }

    float CapsRatio = static_cast<float>(UpperCount) / LetterCount;
    return CapsRatio > Configuration.ContentFilter.CapsThreshold;
}

bool USafetyManager::CheckRepeatedMessage(const FString& UserID, const FString& Content)
{
    if (!Configuration.ContentFilter.bBlockRepeatedMessages)
    {
        return false;
    }

    FString* LastMessage = UserLastMessages.Find(UserID);
    if (LastMessage && *LastMessage == Content)
    {
        return true;
    }

    UserLastMessages.Add(UserID, Content);
    return false;
}

void USafetyManager::LogInteraction(const FString& ActionType, const FString& Username, const FString& Details)
{
    if (!Configuration.bEnableLogging)
    {
        return;
    }

    FSafetyLogEntry Entry;
    Entry.Timestamp = FDateTime::Now();
    Entry.ActionType = ActionType;
    Entry.Username = Username;
    Entry.Details = Details;
    Entry.bWasBlocked = false;

    LogEntries.Add(Entry);

    // Trim log if too large
    while (LogEntries.Num() > Configuration.MaxLogEntries)
    {
        LogEntries.RemoveAt(0);
    }
}

void USafetyManager::LogBlockedAction(const FString& ActionType, const FString& Username, const FString& Details, const FString& Reason)
{
    if (!Configuration.bEnableLogging)
    {
        return;
    }

    FSafetyLogEntry Entry;
    Entry.Timestamp = FDateTime::Now();
    Entry.ActionType = ActionType;
    Entry.Username = Username;
    Entry.Details = Details;
    Entry.bWasBlocked = true;
    Entry.BlockReason = Reason;

    LogEntries.Add(Entry);

    UE_LOG(LogAIDigitalHuman, Log, TEXT("BLOCKED: [%s] %s - %s (Reason: %s)"),
        *ActionType, *Username, *Details, *Reason);

    // Trim log if too large
    while (LogEntries.Num() > Configuration.MaxLogEntries)
    {
        LogEntries.RemoveAt(0);
    }
}

TArray<FSafetyLogEntry> USafetyManager::GetRecentLogs(int32 Count) const
{
    int32 StartIndex = FMath::Max(0, LogEntries.Num() - Count);
    TArray<FSafetyLogEntry> RecentLogs;

    for (int32 i = StartIndex; i < LogEntries.Num(); i++)
    {
        RecentLogs.Add(LogEntries[i]);
    }

    return RecentLogs;
}

bool USafetyManager::ExportLogs(const FString& FilePath) const
{
    FString LogContent;

    for (const FSafetyLogEntry& Entry : LogEntries)
    {
        FString Line = FString::Printf(TEXT("[%s] [%s] %s: %s"),
            *Entry.Timestamp.ToString(),
            *Entry.ActionType,
            *Entry.Username,
            *Entry.Details
        );

        if (Entry.bWasBlocked)
        {
            Line += FString::Printf(TEXT(" (BLOCKED: %s)"), *Entry.BlockReason);
        }

        LogContent += Line + TEXT("\n");
    }

    return FFileHelper::SaveStringToFile(LogContent, *FilePath);
}

void USafetyManager::ClearLogs()
{
    LogEntries.Empty();
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Safety logs cleared"));
}

void USafetyManager::SetConfiguration(const FSafetyConfiguration& Config)
{
    Configuration = Config;
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Safety configuration updated"));
}

void USafetyManager::AddBlacklistedWord(const FString& Word)
{
    Configuration.ContentFilter.BlacklistedWords.AddUnique(Word);
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Added blacklisted word"));
}

void USafetyManager::RemoveBlacklistedWord(const FString& Word)
{
    Configuration.ContentFilter.BlacklistedWords.Remove(Word);
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Removed blacklisted word"));
}

void USafetyManager::BanUser(const FString& UserID, const FString& Reason)
{
    BannedUsers.Add(UserID);
    LogInteraction(TEXT("Ban"), UserID, Reason);
    UE_LOG(LogAIDigitalHuman, Log, TEXT("User banned: %s (Reason: %s)"), *UserID, *Reason);
}

void USafetyManager::UnbanUser(const FString& UserID)
{
    BannedUsers.Remove(UserID);
    LogInteraction(TEXT("Unban"), UserID, TEXT("User unbanned"));
    UE_LOG(LogAIDigitalHuman, Log, TEXT("User unbanned: %s"), *UserID);
}

bool USafetyManager::IsUserBanned(const FString& UserID) const
{
    return BannedUsers.Contains(UserID);
}

TArray<FString> USafetyManager::GetBannedUsers() const
{
    return BannedUsers.Array();
}

void USafetyManager::CleanupRateLimits()
{
    FDateTime OneMinuteAgo = FDateTime::Now() - FTimespan::FromMinutes(1);

    // Clean up old timestamps
    for (auto& Pair : UserMessageTimestamps)
    {
        Pair.Value.RemoveAll([&OneMinuteAgo](const FDateTime& Time)
        {
            return Time < OneMinuteAgo;
        });
    }

    // Remove users with no recent messages
    TArray<FString> ToRemove;
    for (auto& Pair : UserMessageTimestamps)
    {
        if (Pair.Value.Num() == 0)
        {
            ToRemove.Add(Pair.Key);
        }
    }

    for (const FString& UserID : ToRemove)
    {
        UserMessageTimestamps.Remove(UserID);
    }
}
