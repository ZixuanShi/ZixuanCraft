// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EObjectType Type = EObjectType::Empty;

	/** The icon to draw in inventory widget UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* Icon = nullptr;

	/** The mesh to render if the player is grabbing this loot in hand */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent = nullptr;
};
