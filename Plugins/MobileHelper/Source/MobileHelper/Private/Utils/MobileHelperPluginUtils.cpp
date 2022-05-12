// Copyright Epic Games, Inc. All Rights Reserved.

#include "Utils/MobileHelperPluginUtils.h"

FString UMobileHelperPluginUtils::GetPluginVersionName(FString PluginName)
{
	FString VersionName = "";

	IPluginManager& PluginManager = IPluginManager::Get();
	TArray<TSharedRef<IPlugin>> Plugins = PluginManager.GetEnabledPlugins();
	for (const TSharedRef<IPlugin>& Plugin : Plugins)
	{
		if (Plugin->GetName() == PluginName)
		{
			const FPluginDescriptor& Descriptor = Plugin->GetDescriptor();
			VersionName = Descriptor.VersionName;
		}
	}

	return VersionName;
}