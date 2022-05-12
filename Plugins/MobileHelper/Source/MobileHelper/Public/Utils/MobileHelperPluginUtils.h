// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Interfaces/IPluginManager.h"
#include "PluginDescriptor.h"

#include "MobileHelperPluginUtils.generated.h"

UCLASS()
class UMobileHelperPluginUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Get a string of Plugin Version name
	 */
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|Plugin Utils")
	static FString GetPluginVersionName(FString PluginName);
};

