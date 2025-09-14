// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CJSlate : ModuleRules
{
	public CJSlate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "GameplayTags"});
		// 추가 라이브러리
		// PrivateDependencyModuleNames.AddRange(new string[] { "MaterialEditor" });
		PrivateDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore"});
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" }); // CreateSession등을 쓰기 위함.
		
		// Custom 라이브러리
		PrivateDependencyModuleNames.AddRange(new string[] { "CJLibrary" });
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("CJUIEditor");
		}
	}
}
