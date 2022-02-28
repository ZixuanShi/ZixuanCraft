// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainManager.generated.h"

class ATerrainVoxel;

/**
* Responsible for generating terrains. Should be only one instance in the world
*/
UCLASS()
class ZIXUANCRAFT_API ATerrainManager : public AActor
{
	GENERATED_BODY()

private:
	/** Custom seed for generating the terrains */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Seed = 0;

	/** How many voxels to render in X/Y coordinates at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 RenderRadius = 4;

	/** How many cubes in XY coordinate for a voxel to hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountXY = 20;

	/** How many cubes in Z coordinate for a voxel to hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountZ = 80;

	/** The threshold to render grass/empty materials. Must not be greater than CubeCountZ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 GrassThreshold = 30;

	/** The offset to render stones, any thing below GrassThreshold - StoneOffset will be stones */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 StoneOffset = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float GenerateTreeChance = 0.002f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Weight = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpawnObjectChance = 0.003f;

	/** Length of a single cube for XY axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CubeLength = 100.0f;

	/** Length of an entire voxel for XY axis */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float VoxelLength = TNumericLimits<float>::Max();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 PlayerAtCubeX = TNumericLimits<int32>::Max();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 PlayerAtCubeY = TNumericLimits<int32>::Max();
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ATerrainVoxel*> Terrains;

	/** 
	 * FVector for the voxel's location as key, int32 as the index in Terrains TArray as value. 
	 * Used when adding/removing voxels when the player moves and when the player place/destroy a cube
	 */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FVector, int32> TerrainLocations;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ATerrainVoxel* SpawnedVoxel = nullptr;

public:
	ATerrainManager();

	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;

	float GetCubeLength() const { return CubeLength; }
	float GetVoxelLength() const { return VoxelLength; }
	const TMap<FVector, int32>& GetTerrainLocations() const { return TerrainLocations; }
	const TArray<ATerrainVoxel*>& GetTerrains() const { return Terrains; }

private:
	bool UpdatedPosition();
	void UpdateTerrains();
	void AddChunk();
	void RemoveChunk();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnVoxel(FVector InVoxelLocation, int32 X, int32 Y);
};
