// Fill out your copyright notice in the Description page of Project Settings.
// ZixuanCraftInventoryButton.h
// Author: Zixuan Shi
// A button used for InventoryUI widget

#pragma once
#include "UI/ZixuanCraftButton.h"

#include "ZixuanCraftInventoryButton.generated.h"

/**
 * A button of inventory panel in the main game widget
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftInventoryButton : public UZixuanCraftButton
{
	GENERATED_BODY()

protected:
	/** On clicked, update player's inventory component if needed */
	virtual void OnFirstPressedImpl() override final;
	virtual void OnSecondPressedImpl() override final;
};
