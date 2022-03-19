// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "GameObjects/Loot/Loot.h"

PRAGMA_DISABLE_OPTIMIZATION

bool UInventoryComponent::TryAdd(ALoot* Loot)
{
	// Find if there is a existing loot slot for this loot. Add this loot to that slot if it's not full
	for (FLootSlot& LootSlot : Inventory)
	{
		if (LootSlot.TryAppend(Loot))
		{
			return true;
		}		
	}

	// There is no existing loot slot for this type, create a new one if this inventory is not full
	if (Inventory.Num() <= MaxSize)
	{
		Inventory.Emplace();						// Emplace a new slot
		Inventory.Last().AddFirstLoot(Loot);
		return true;
	}

	// At this point, we didn't find a existing valid loot slot to append the new loot
	// and ran out of space in this inventory, we can't add this loot
	return false;
}

PRAGMA_ENABLE_OPTIMIZATION