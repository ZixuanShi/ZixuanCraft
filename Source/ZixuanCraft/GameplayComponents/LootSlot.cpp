// Fill out your copyright notice in the Description page of Project Settings.


#include "LootSlot.h"
#include "GameObjects/Loot/Loot.h"

bool FLootSlot::TryAppend(ALoot* NewLoot)
{
	if (Type > EObjectType::RoastedChicken ||	// Not Stackable?
		Count >= MaxSlotCount ||				// Slot full?
		Type != NewLoot->GetType())				// Not Same type?
	{
		return false;
	}

	// Success
	++Count;
	return true;
}

void FLootSlot::AddFirstLoot(ALoot* NewLoot)
{
	Type		  = NewLoot->GetType();	
	Icon		  = NewLoot->GetIcon();
	MeshComponent = NewLoot->GetMesh();
	++Count;					
}
