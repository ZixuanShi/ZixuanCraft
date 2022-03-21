// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "GameplayComponents/LootSlot.h"
#include "ZixuanCraftInventoryButton.generated.h"

class UTextBlock;

/**
 * A button of inventory panel in the main game widget
 * - If there is no item in the inventory button, show a gray rectangle
 * - If there is a item in the corresponding inventory button, it's background should show the loot slot item's image, and a number indicates the quantity of that loot
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftInventoryButton : public UButton
{
	GENERATED_BODY()
	
protected:
	/** The quantity of this loot */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CountText = nullptr;

	/** The index in the inventory panel */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Index = 0;

public:
	void Init(int32 NewIndex);

	UFUNCTION()
	void Update(const FLootSlot& InSlot);

	UFUNCTION()
	void Select();
};
