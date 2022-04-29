// Fill out your copyright notice in the Description page of Project Settings.
// LootSlot.h
// Author: Zixuan Shi
// A struct represents a slot in inventory UI and component

#pragma once
#include "GameObjects/ObjectEnums.h"
#include "GameObjects/Loot/LootData.h"
#include "LootSlot.generated.h"

class ALoot;

/**
 * Represents a slot for loots in inventory.
 */
USTRUCT(BlueprintType)
struct ZIXUANCRAFT_API FLootSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLootData LootData;

	/** How many loot of this type in this slot. If the loot is stackable, this will grow when inventory component wants to add that type until hit the MaxSlotCount. If the loot is not stackable, this can only be one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;

	/**
	 * Try to append a new stackable loot
	 * @param Loot		The loot trying to append
	 * @return			true if succeeded, false if not
	 */
	bool TryAppend(ALoot* NewLoot);

	/**
	 * Try to add a new loot as the first item in this slot
	 * @param Loot		The loot trying to add
	 * @return			true if succeeded, false if not
	 */
	void AddFirstLoot(ALoot* NewLoot);

	/** Reset all members back to the default ones */
	void Reset();
};
