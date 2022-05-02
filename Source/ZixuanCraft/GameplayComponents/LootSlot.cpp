// Fill out your copyright notice in the Description page of Project Settings.


#include "LootSlot.h"
#include "GameObjects/Loot/Loot.h"
#include "Utils/TypeDefs.h"

bool FLootSlot::TryAppend(ALoot* NewLoot)
{
	if (LootData.Type > EObjectType::RoastedChicken ||	// Not Stackable?
		LootData.Type != NewLoot->GetLootData().Type ||	// Not Same type?
		Count >= MaxSlotCount)							// Slot full?
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

void FLootSlot::Reset()
{
	LootData.Reset();
	Count = 0;
}
