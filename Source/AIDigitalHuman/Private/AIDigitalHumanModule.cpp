// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "AIDigitalHumanModule.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogAIDigitalHuman);

#define LOCTEXT_NAMESPACE "FAIDigitalHumanModule"

void FAIDigitalHumanModule::StartupModule()
{
    UE_LOG(LogAIDigitalHuman, Log, TEXT("AI Digital Human Module Starting..."));

    InitializeSubsystems();

    UE_LOG(LogAIDigitalHuman, Log, TEXT("AI Digital Human Module Started Successfully"));
}

void FAIDigitalHumanModule::ShutdownModule()
{
    UE_LOG(LogAIDigitalHuman, Log, TEXT("AI Digital Human Module Shutting Down..."));

    ShutdownSubsystems();

    UE_LOG(LogAIDigitalHuman, Log, TEXT("AI Digital Human Module Shutdown Complete"));
}

FAIDigitalHumanModule& FAIDigitalHumanModule::Get()
{
    return FModuleManager::LoadModuleChecked<FAIDigitalHumanModule>("AIDigitalHuman");
}

bool FAIDigitalHumanModule::IsAvailable()
{
    return FModuleManager::Get().IsModuleLoaded("AIDigitalHuman");
}

void FAIDigitalHumanModule::InitializeSubsystems()
{
    // Subsystem initialization happens through UGameInstanceSubsystem
    // and UWorldSubsystem classes registered via the module
}

void FAIDigitalHumanModule::ShutdownSubsystems()
{
    // Cleanup handled by subsystem destructors
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAIDigitalHumanModule, AIDigitalHuman)
