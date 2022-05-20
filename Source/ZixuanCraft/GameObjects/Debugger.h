// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Debugger.generated.h"

/**
 * Dedicated for debugging
 */
UCLASS()
class ZIXUANCRAFT_API ADebugger : public APawn
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AnyKeyPressed(const FKey& InKey);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AnyKeyReleased(const FKey& InKey);
};
