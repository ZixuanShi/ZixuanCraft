// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Inventory.Reserve(MaxSize);
}

void UInventoryComponent::AddLoot(ALoot* Loot)
{
	Inventory.Emplace(Loot);
}

