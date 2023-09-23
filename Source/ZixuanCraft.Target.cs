// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ZixuanCraftTarget : TargetRules
{
	public ZixuanCraftTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		ExtraModuleNames.Add("ZixuanCraft");

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			ExtraModuleNames.Add("OnlineSubsystemGooglePlay");
			ExtraModuleNames.Add("OnlineSubsystem");
			ExtraModuleNames.Add("AndroidAdvertising");
		}
	}
}
