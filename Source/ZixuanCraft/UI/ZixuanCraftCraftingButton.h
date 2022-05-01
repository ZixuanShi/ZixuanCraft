// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZixuanCraftButton.h"
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
	void SetCraftingManager(ACraftingManager* InCraftingManager) { CraftingManager = InCraftingManager; }

protected:
	virtual void OnLeftMouseFirstPressedImpl() override final;
	virtual void OnLeftMouseSecondPressedImpl() override final;
};
