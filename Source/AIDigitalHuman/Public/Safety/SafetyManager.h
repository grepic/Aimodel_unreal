// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "SafetyManager.generated.h"

/**
 * Safety action log entry
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FSafetyLogEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Safety")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Safety")
    FString ActionType;

    UPROPERTY(BlueprintReadOnly, Category = "Safety")
    FString Username;

    UPROPERTY(BlueprintReadOnly, Category = "Safety")
    FString Details;

    UPROPERTY(BlueprintReadOnly, Category = "Safety")
    bool bWasBlocked = false;

    UPROPERTY(BlueprintReadOnly, Category = "Safety")
    FString BlockReason;
};

/**
 * Content filter configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FContentFilterConfig
{
    GENERATED_BODY()

    /** Enable content filtering */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    bool bEnabled = true;

    /** Block messages containing blacklisted words */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    TArray<FString> BlacklistedWords;

    /** Block messages containing blacklisted patterns (regex) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    TArray<FString> BlacklistedPatterns;

    /** Maximum message length */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    int32 MaxMessageLength = 500;

    /** Block messages with excessive caps */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    bool bBlockExcessiveCaps = true;

    /** Caps threshold (0-1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    float CapsThreshold = 0.7f;

    /** Block repeated messages */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    bool bBlockRepeatedMessages = true;

    /** Use external moderation API */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    bool bUseExternalModeration = false;

    /** External moderation API endpoint */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
    FString ExternalModerationEndpoint;
};

/**
 * Safety configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FSafetyConfiguration
{
    GENERATED_BODY()

    /** This is an adult content platform */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    bool bAdultContentPlatform = true;

    /** Display AI disclosure */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    bool bShowAIDisclosure = true;

    /** AI disclosure text */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    FString AIDisclosureText = TEXT("This is an AI-generated digital human, not a real person.");

    /** Enable action logging */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    bool bEnableLogging = true;

    /** Maximum log entries to keep in memory */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    int32 MaxLogEntries = 10000;

    /** Enable rate limiting */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    bool bEnableRateLimiting = true;

    /** Maximum messages per user per minute */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    int32 MaxMessagesPerMinute = 10;

    /** Content filter configuration */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safety")
    FContentFilterConfig ContentFilter;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmergencyStopActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSystemResumed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageBlocked, const FString&, Username, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUserRateLimited, const FString&, Username, int32, MessageCount);

/**
 * Manages safety features, content filtering, and logging.
 * Provides emergency stop, moderation hooks, and AI disclosure.
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API USafetyManager : public UObject
{
    GENERATED_BODY()

public:
    USafetyManager();

    /** Initialize the safety manager */
    UFUNCTION(BlueprintCallable, Category = "Safety")
    void Initialize();

    // ==================== Emergency Controls ====================

    /** Activate emergency stop */
    UFUNCTION(BlueprintCallable, Category = "Safety|Emergency")
    void ActivateEmergencyStop();

    /** Resume from emergency stop */
    UFUNCTION(BlueprintCallable, Category = "Safety|Emergency")
    void Resume();

    /** Check if in emergency stop state */
    UFUNCTION(BlueprintCallable, Category = "Safety|Emergency")
    bool IsEmergencyStopped() const { return bEmergencyStopped; }

    // ==================== Content Validation ====================

    /** Validate a chat message */
    UFUNCTION(BlueprintCallable, Category = "Safety|Validation")
    bool ValidateMessage(const FChatMessage& Message);

    /** Validate text content */
    UFUNCTION(BlueprintCallable, Category = "Safety|Validation")
    bool ValidateContent(const FString& Content, FString& OutReason);

    /** Check rate limit for user */
    UFUNCTION(BlueprintCallable, Category = "Safety|Validation")
    bool CheckRateLimit(const FString& UserID);

    // ==================== Logging ====================

    /** Log an interaction */
    UFUNCTION(BlueprintCallable, Category = "Safety|Logging")
    void LogInteraction(const FString& ActionType, const FString& Username, const FString& Details);

    /** Log a blocked action */
    UFUNCTION(BlueprintCallable, Category = "Safety|Logging")
    void LogBlockedAction(const FString& ActionType, const FString& Username, const FString& Details, const FString& Reason);

    /** Get recent log entries */
    UFUNCTION(BlueprintCallable, Category = "Safety|Logging")
    TArray<FSafetyLogEntry> GetRecentLogs(int32 Count = 100) const;

    /** Export logs to file */
    UFUNCTION(BlueprintCallable, Category = "Safety|Logging")
    bool ExportLogs(const FString& FilePath) const;

    /** Clear logs */
    UFUNCTION(BlueprintCallable, Category = "Safety|Logging")
    void ClearLogs();

    // ==================== Configuration ====================

    /** Set safety configuration */
    UFUNCTION(BlueprintCallable, Category = "Safety|Config")
    void SetConfiguration(const FSafetyConfiguration& Config);

    /** Get current configuration */
    UFUNCTION(BlueprintCallable, Category = "Safety|Config")
    FSafetyConfiguration GetConfiguration() const { return Configuration; }

    /** Add word to blacklist */
    UFUNCTION(BlueprintCallable, Category = "Safety|Config")
    void AddBlacklistedWord(const FString& Word);

    /** Remove word from blacklist */
    UFUNCTION(BlueprintCallable, Category = "Safety|Config")
    void RemoveBlacklistedWord(const FString& Word);

    // ==================== User Management ====================

    /** Ban a user */
    UFUNCTION(BlueprintCallable, Category = "Safety|Users")
    void BanUser(const FString& UserID, const FString& Reason);

    /** Unban a user */
    UFUNCTION(BlueprintCallable, Category = "Safety|Users")
    void UnbanUser(const FString& UserID);

    /** Check if user is banned */
    UFUNCTION(BlueprintCallable, Category = "Safety|Users")
    bool IsUserBanned(const FString& UserID) const;

    /** Get banned users list */
    UFUNCTION(BlueprintCallable, Category = "Safety|Users")
    TArray<FString> GetBannedUsers() const;

    // ==================== AI Disclosure ====================

    /** Get AI disclosure text */
    UFUNCTION(BlueprintCallable, Category = "Safety|Disclosure")
    FString GetAIDisclosureText() const { return Configuration.AIDisclosureText; }

    /** Check if AI disclosure should be shown */
    UFUNCTION(BlueprintCallable, Category = "Safety|Disclosure")
    bool ShouldShowAIDisclosure() const { return Configuration.bShowAIDisclosure; }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Safety|Events")
    FOnEmergencyStopActivated OnEmergencyStopActivated;

    UPROPERTY(BlueprintAssignable, Category = "Safety|Events")
    FOnSystemResumed OnSystemResumed;

    UPROPERTY(BlueprintAssignable, Category = "Safety|Events")
    FOnMessageBlocked OnMessageBlocked;

    UPROPERTY(BlueprintAssignable, Category = "Safety|Events")
    FOnUserRateLimited OnUserRateLimited;

protected:
    /** Check content against blacklist */
    bool CheckBlacklist(const FString& Content, FString& OutMatchedWord) const;

    /** Check for excessive caps */
    bool CheckExcessiveCaps(const FString& Content) const;

    /** Check for repeated message */
    bool CheckRepeatedMessage(const FString& UserID, const FString& Content);

    /** Update rate limit tracking */
    void UpdateRateLimiting(const FString& UserID);

    /** Clean up old rate limit entries */
    void CleanupRateLimits();

private:
    FSafetyConfiguration Configuration;

    // State
    bool bIsInitialized = false;
    bool bEmergencyStopped = false;

    // Logging
    TArray<FSafetyLogEntry> LogEntries;

    // Rate limiting: UserID -> (Timestamps of recent messages)
    TMap<FString, TArray<FDateTime>> UserMessageTimestamps;

    // Previous messages for repeat detection: UserID -> LastMessage
    TMap<FString, FString> UserLastMessages;

    // Banned users
    TSet<FString> BannedUsers;

    // Timers
    FTimerHandle CleanupTimer;
};
