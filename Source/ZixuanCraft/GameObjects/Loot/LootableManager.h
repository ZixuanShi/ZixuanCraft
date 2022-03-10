// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjects/Terrain/TerrainEnums.h"
#include "LootableManager.generated.h"

class ALootable;

/**
 * Managing spawning Lootable in the world. Consider this a factory pattern. Should be only one instance in the world
 */
UCLASS()
class ZIXUANCRAFT_API ALootableManager : public AActor
{
	GENERATED_BODY()

	friend ALootable;
	
public:
	/**
	 * Spawn loot for a terrain cube.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnTerrainCubeLoot(ECubeType CubeType);
};
