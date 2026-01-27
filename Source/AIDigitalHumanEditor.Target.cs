// Copyright 2024 AI Digital Human Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AIDigitalHumanEditorTarget : TargetRules
{
    public AIDigitalHumanEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("AIDigitalHuman");

        // Enable C++20 features
        CppStandard = CppStandardVersion.Cpp20;
    }
}
