// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainEnums.h"
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
	TArray<ECubeType> AllCubes;
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

	UFUNCTION(BlueprintCallable)
	void SetVoxel(FVector Location, ECubeType NewType);

private:
	/** Create voxel mesh information and determine the type of a cube */
	void GenerateChunk();
	void UpdateMesh();
	TArray<FMeshSection> GenerateMeshSections();
	void UpdateSingleCube(int32 X, int32 Y, int32 Z, TArray<FMeshSection>& MeshSections);
	void RegenerateMesh(const TArray<FMeshSection>& MeshSections);
	void ApplyMaterials();

	/** Vertices */
	void HandleNonEmptyCube(int32 X, int32 Y, int32 Z, const ECubeType CubeType, TArray<FMeshSection>& MeshSections);
	void AddVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, int32 X, int32 Y, int32 Z, int32 CubeSideIndex);

protected:
	/** Calculate perlin noise value for terrain generation */
	UFUNCTION(BlueprintCallable)
	void CalculateNoise();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnNPC(FVector Location);
};
