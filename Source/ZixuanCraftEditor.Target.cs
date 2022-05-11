// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ZixuanCraftEditorTarget : TargetRules
{
	public ZixuanCraftEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("ZixuanCraft");

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			ExtraModuleNames.Add("OnlineSubsystemGooglePlay");
			ExtraModuleNames.Add("OnlineSubsystem");
			ExtraModuleNames.Add("AndroidAdvertising");
		}
	}
}
