// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GooglePlayServicesBPLibrary.generated.h"

/* 
 * GooglePlay Services Toolbox for Unreal Engine
 */
UCLASS()
class UGooglePlayServicesBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Login", Keywords = "Login"), Category = "GooglePlayServices")
	static void Login();
};
