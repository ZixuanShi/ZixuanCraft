// Fill out your copyright notice in the Description page of Project Settings.


#include "LootSlot.h"
#include "GameObjects/Loot/Loot.h"

bool FLootSlot::TryAppend(ALoot* NewLoot)
{
	if (LootData.Type > EObjectType::RoastedChicken ||	// Not Stackable?
		Count >= MaxSlotCount ||				// Slot full?
		LootData.Type != NewLoot->GetLootData().Type)				// Not Same type?
	{
		return false;
	}

	// Success
	++Count;
	return true;
}

void FLootSlot::AddFirstLoot(ALoot* NewLoot)
{
	LootData = NewLoot->GetLootData();
	++Count;					
}
