// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMobileHelper, Verbose, All);

class UMobileHelperSettings;

class FMobileHelperModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Singleton for loading module
	*/
	static FMobileHelperModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FMobileHelperModule>("MobileHelper");
	}

	/**
	* Checks to see if this module is loaded and ready. It is only valid to call Get() if IsAvailable() returns true.
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("MobileHelper");
	}

	UMobileHelperSettings* GetSettings() const { return MobileHelperSettings; }

private:

	UMobileHelperSettings* MobileHelperSettings;
};
