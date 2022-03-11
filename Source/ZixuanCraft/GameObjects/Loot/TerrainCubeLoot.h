// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Loot.h"
#include "GameObjects/Terrain/TerrainEnums.h"
#include "GameObjects/Terrain/MeshSection.h"
#include "TerrainCubeLoot.generated.h"

/**
 * Loot of a solid terrain cube such as dirt, stone, tree trunk, not for tree leaves. 
 * Using ProceduralMeshComponent
 */
UCLASS()
class ZIXUANCRAFT_API ATerrainCubeLoot : public ALoot
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	ECubeType CubeType = ECubeType::Dirt;

public:
	ATerrainCubeLoot();

	void SetCubeType(ECubeType InCubeType) { CubeType = InCubeType; }
};
