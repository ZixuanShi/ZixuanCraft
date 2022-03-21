// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZixuanCraft : ModuleRules
{
	public ZixuanCraft(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "SimplexNoise" });
		PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent", "Slate", "SlateCore" });
		PrivateIncludePaths.AddRange(new string[] {	"ZixuanCraft"	});
	}
}
