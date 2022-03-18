// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Loot.h"
#include "TerrainCubeLoot.generated.h"

/**
 * Loot of a solid terrain cube such as dirt, stone, tree trunk, not for tree leaves. 
 */
UCLASS()
class ZIXUANCRAFT_API ATerrainCubeLoot : public ALoot
{
	GENERATED_BODY()

public:
	ATerrainCubeLoot();
};
