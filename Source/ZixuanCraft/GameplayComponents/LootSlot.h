// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticArray.h"
#include "GameObjects/ObjectEnums.h"
#include "LootSlot.generated.h"

class ALoot;

/**
 * Represents a slot for loots in inventory.
 */
USTRUCT(BlueprintType)
struct ZIXUANCRAFT_API FLootSlot
{
	GENERATED_BODY()

	/** Represents the Loot type in this slot. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EObjectType Type = EObjectType::Empty;

	/** How many loot of this type in this slot. If the loot is stackable, this will grow when inventory component wants to add that type until hit the MaxSlotCount. If the loot is not stackable, this can only be one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Count = 0;

	/** The icon to draw in inventory widget UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* Icon = nullptr;

	/** The mesh to render if the player is grabbing this loot in hand */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent = nullptr;

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
};
