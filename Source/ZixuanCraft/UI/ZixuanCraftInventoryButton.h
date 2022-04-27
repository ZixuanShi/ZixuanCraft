// Fill out your copyright notice in the Description page of Project Settings.
// ZixuanCraftInventoryButton.h
// Author: Zixuan Shi
// A button used for InventoryUI widget

#pragma once
#include "UI/ZixuanCraftButton.h"

#include "CoreMinimal.h"
#include "ZixuanCraftInventoryButton.generated.h"

/**
 * A button of inventory panel in the main game widget
 * - If there is no item in the inventory button, show a gray rectangle
 * - If there is a item in the corresponding inventory button, it's background should show the loot slot item's image, and a number indicates the quantity of that loot
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftInventoryButton : public UZixuanCraftButton
{
	GENERATED_BODY()
	
protected:
	/** The index in the inventory panel */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Index = 0;

public:
	void Init(int32 NewIndex);
	void Update(const FLootSlot& InSlot);
	void Highlight();
	void Reset();

	UFUNCTION()
	void OnSelected();
};
