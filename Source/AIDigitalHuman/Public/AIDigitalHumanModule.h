// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIDigitalHuman, Log, All);

/**
 * Main module for the AI Digital Human system.
 * Manages initialization and shutdown of all subsystems.
 */
class FAIDigitalHumanModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /**
     * Singleton-like access to this module's interface.
     * @return Returns singleton instance, loading the module on demand if needed.
     */
    static FAIDigitalHumanModule& Get();

    /**
     * Checks if the module is loaded and ready.
     * @return True if the module is loaded.
     */
    static bool IsAvailable();

private:
    /** Initialize all subsystems */
    void InitializeSubsystems();

    /** Shutdown all subsystems */
    void ShutdownSubsystems();
};
