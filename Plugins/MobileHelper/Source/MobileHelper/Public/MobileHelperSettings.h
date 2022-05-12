// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/EngineTypes.h"
#include "MobileHelperSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class MOBILEHELPER_API UMobileHelperSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	FString ClientID;
};
