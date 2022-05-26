// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Gameplay/Buttons/ZixuanCraftCraftingButton.h"
#include "ZixuanCraftCraftingResultButton.generated.h"

/**
 * Dedicated button for crafting result
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftCraftingResultButton : public UZixuanCraftCraftingButton
{
	GENERATED_BODY()
	
public:
	virtual void OnRightMousePressed() override final {}

protected:
	/** On clicked, update player's inventory component if needed */
	virtual void OnLeftMouseFirstPressedImpl() override final;
	virtual void OnLeftMouseSecondPressedImpl() override final {}
};
