// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayComponents/LootableComponent.h"
#include "GameObjects/Loot/Loot.h"
PRAGMA_DISABLE_OPTIMIZATION

void ULootableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (const FLootSlot& LootSlot : LootsToDrop)
	{
		for (int32 i = 0; i < LootSlot.Count; ++i)
		{
			ALoot* Loot = GetWorld()->SpawnActor<ALoot>(GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
			if (Loot)
			{
				Loot->SetLootData(LootSlot.LootData);
			}
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION