// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GPPermissionFlags.generated.h"

USTRUCT(BlueprintType)
struct MOBILEHELPER_API FGPPermissionFlags
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Mobile Helper GooglePlayServices")
	bool bId;

	UPROPERTY(BlueprintReadWrite, Category = "Mobile Helper GooglePlayServices")
	bool bIdToken;

	UPROPERTY(BlueprintReadWrite, Category = "Mobile Helper GooglePlayServices")
	bool bProfile;

	UPROPERTY(BlueprintReadWrite, Category = "Mobile Helper GooglePlayServices")
	bool bEmail;

	UPROPERTY(BlueprintReadWrite, Category = "Mobile Helper GooglePlayServices")
	bool bCloudSave;
};
