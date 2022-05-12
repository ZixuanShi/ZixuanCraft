// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileHelper.h"
#include "MobileHelperSettings.h"
#include "Utils/MobileHelperPluginUtils.h"

#include "Engine/Engine.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FMobileHelperModule"

DEFINE_LOG_CATEGORY(LogMobileHelper)

void FMobileHelperModule::StartupModule()
{
	FString PluginName = "MobileHelper";

	MobileHelperSettings = NewObject<UMobileHelperSettings>(GetTransientPackage(), "MobileHelperSettings", RF_Standalone);
	MobileHelperSettings->AddToRoot();

	// Register settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "MobileHelper",
			LOCTEXT("RuntimeSettingsName", "Mobile Helper"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Mobile Helper"),
			MobileHelperSettings);
	}

	const FString PluginVersionName = UMobileHelperPluginUtils::GetPluginVersionName(PluginName);
	UE_LOG(LogMobileHelper, Verbose, TEXT("[%s] - Version: %s"), *PluginName, *PluginVersionName);
}

void FMobileHelperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMobileHelperModule, MobileHelper)