// Fill out your copyright notice in the Description page of Project Settings.
// LootData.h
// Author: Zixuan Shi
// Struct for loot data

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/ObjectEnums.h"
#include "LootData.generated.h"

/**
 * Data for a single loot
 */
USTRUCT(BlueprintType)
struct ZIXUANCRAFT_API FLootData
{
	GENERATED_BODY()

	/** Represents the Loot type in this slot. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectType Type = EObjectType::Empty;

	/** The icon to draw in inventory widget UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	/** The mesh to render if the player is grabbing this loot in hand */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent = nullptr;

	/** Reset all members back to the default ones */
	void Reset();
};
