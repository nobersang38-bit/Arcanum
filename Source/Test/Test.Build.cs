// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Test : ModuleRules
{
	public Test(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "DeveloperSettings", "GameplayTags",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });


        PublicIncludePaths.AddRange(new string[] { "Test/Public" });
        PrivateIncludePaths.AddRange(new string[] { "Test/Private" });
    }
}
