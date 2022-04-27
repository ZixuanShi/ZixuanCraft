// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayComponents/LootSlot.h"

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ZixuanCraftButton.generated.h"

class UTextBlock;

/**
 * Base class for UI button widget used in the game UI system
 * It has an image and a count text indicating the item and it's quantity
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftButton : public UButton
{
	GENERATED_BODY()

protected:
	/** Data for this button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	FLootSlot Data;

	/** The quantity text of this loot */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CountText = nullptr;

public:
	UZixuanCraftButton();
};
