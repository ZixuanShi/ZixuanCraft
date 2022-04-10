// Fill out your copyright notice in the Description page of Project Settings.
// TerrainVoxel.h
// Author: Zixuan Shi
// Game object with procedural mesh component, holding a portion of cubes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjects/ObjectEnums.h"
#include "MeshSection.h"
#include "Utils/TypeDefs.h"
#include "Utils/RNG.h"
#include "TerrainVoxel.generated.h"

class UProceduralMeshComponent;
class ATerrainManager;

/**
 * Voxel holding the terrain cubes
 */
UCLASS()
class ZIXUANCRAFT_API ATerrainVoxel : public AActor
{
	GENERATED_BODY()

private:
	TArray<EObjectType> AllCubes;
	TArray<int32> NoiseResult;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> SpawnedObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	ATerrainManager* TerrainManager = nullptr;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 VoxelX = 0;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 VoxelY = 0;

public:	
	ATerrainVoxel();

	virtual void OnConstruction(const FTransform& Transform) override final;

	void ModifyCube(FVector CubeLocation, FVector SpawnLootLocation, EObjectType NewType);

private:
	/** Create voxel mesh information and determine the type of a cube */
	void GenerateChunk();
	void UpdateMesh();
	TArray<FMeshSection> GenerateMeshSections();
	void UpdateSingleCube(int32 X, int32 Y, int32 Z, TArray<FMeshSection>& MeshSections);
	void RegenerateMesh(const TArray<FMeshSection>& MeshSections);
	void ApplyMaterials();

	/** Vertices */
	void HandleNonEmptyCube(int32 X, int32 Y, int32 Z, const EObjectType ObjectType, TArray<FMeshSection>& MeshSections);
	void AddVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, int32 X, int32 Y, int32 Z, int32 CubeSideIndex);

protected:
	/** Calculate perlin noise value for terrain generation */
	UFUNCTION(BlueprintCallable)
	void CalculateNoise();

	/** Spawning */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnNPC(FVector Location);
};
