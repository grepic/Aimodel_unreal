// Copyright 2024 AI Digital Human Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AIDigitalHumanTarget : TargetRules
{
    public AIDigitalHumanTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("AIDigitalHuman");

        // Enable hardware ray tracing for Lumen
        bUseLoggingInShipping = true;

        // Optimizations for streaming
        bCompilePhysX = true;
        bCompileNvCloth = false;
        bCompileAPEX = false;

        // Enable C++20 features
        CppStandard = CppStandardVersion.Cpp20;
    }
}
