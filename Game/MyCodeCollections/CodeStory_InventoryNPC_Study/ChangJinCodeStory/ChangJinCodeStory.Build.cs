// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChangJinCodeStory : ModuleRules
{
	public ChangJinCodeStory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" , "AIModule", "GameplayTasks", "UMG", "Slate", "SlateCore"});
	}
}
