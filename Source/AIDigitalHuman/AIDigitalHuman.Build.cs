// Copyright 2024 AI Digital Human Project. All Rights Reserved.

using UnrealBuildTool;

public class AIDigitalHuman : ModuleRules
{
    public AIDigitalHuman(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Enable C++20 features
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            // Core Engine
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",

            // Animation
            "AnimGraphRuntime",
            "ControlRig",
            "RigVM",
            "RigLogicModule",
            "FullBodyIK",

            // AI
            "AIModule",
            "GameplayTasks",

            // Rendering
            "RenderCore",
            "RHI",
            "Renderer",

            // Audio
            "AudioMixer",
            "AudioCapture",

            // Networking
            "WebSockets",
            "HTTP",
            "Json",
            "JsonUtilities",

            // Media
            "MediaAssets",
            "MediaIOCore",

            // MetaHuman
            "MetaHumanMeshTracker",

            // LiveLink
            "LiveLinkInterface",
            "LiveLink",

            // Niagara (for hair/groom)
            "Niagara",
            "NiagaraCore",

            // Slate UI
            "Slate",
            "SlateCore",
            "UMG"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "HairStrandsCore",
            "Groom",
            "VirtualCamera",
            "CinematicCamera"
        });

        // Platform-specific modules
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDependencyModuleNames.AddRange(new string[]
            {
                "DLSS",
                "StreamlineCore"
            });
        }

        // Enable exceptions for external API handling
        bEnableExceptions = true;

        // RTTI for dynamic casting in AI systems
        bUseRTTI = true;
    }
}
