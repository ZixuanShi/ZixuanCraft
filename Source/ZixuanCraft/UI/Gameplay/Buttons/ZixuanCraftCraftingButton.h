// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Gameplay/Buttons/ZixuanCraftButton.h"
#include "ZixuanCraftCraftingButton.generated.h"

class ACraftingManager;

/**
 * Button used for crafting
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftCraftingButton : public UZixuanCraftButton
{
	GENERATED_BODY()

protected:
	ACraftingManager* CraftingManager = nullptr;

public:
	virtual void OnRightMousePressed() override;
	
	void SetCraftingManager(ACraftingManager* InCraftingManager) { CraftingManager = InCraftingManager; }

protected:
	virtual void OnLeftMouseFirstPressedImpl() override;
	virtual void OnLeftMouseSecondPressedImpl() override;

	/** Update the crafting manager to the latest crafting panel status */
	void NotifyCraftingManager();
};
