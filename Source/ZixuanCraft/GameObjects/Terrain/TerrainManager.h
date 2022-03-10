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

	friend ATerrainVoxel;

private:
	/** Stores all the voxels */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ATerrainVoxel*> Terrains;

	/** Materials' order must perfectly match ECubeType in TerrainVoxel.h */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInterface*> Materials;

	/**
	 * FVector for the voxel's location as key, int32 as the index in Terrains TArray as value.
	 * Used when adding/removing voxels when the player moves and when the player place/destroy a cube
	 */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FVector, bool> TerrainLocations;

	/** The voxel spawned recently */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ATerrainVoxel* SpawnedVoxel = nullptr;

	/** Custom seed for generating the terrains */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Seed = 19990726;

	/** How many voxels to render in X/Y coordinates at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 RenderRadius = 4;

	/** How many cubes in XY coordinate for a voxel to hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountXY = 20;

	UPROPERTY(BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountXYSquared = CubeCountXY * CubeCountXY;

	/** How many cubes in Z coordinate for a voxel to hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountZ = 64;

	/** The threshold to render grass/empty materials. Must not be greater than CubeCountZ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 GrassThreshold = 30;

	/** The offset to render stones, any thing below GrassThreshold - StoneOffset will be stones */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 StoneOffset = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Weight = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpawnObjectChance = 0.003f;

	/** Length of a single cube */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CubeLength = 100.0f;

	UPROPERTY(BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float CubeLengthHalf = 0.0f;

	/** Length of an entire voxel for XY */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float VoxelLength = TNumericLimits<float>::Max();

	/** Where the player current at in X axis */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 PlayerAtCubeX = TNumericLimits<int32>::Max();

	/** Where the player current at in Y axis */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 PlayerAtCubeY = TNumericLimits<int32>::Max();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpawnTreeChance = 0.002f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D TreeHeightRange = { 6.0f, 12.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D LeavesLengthRange = { 3.0f, 6.0f };

	/** How plump the leaves are. Value in 0 to 1 range, the greater the plumper the leaves are */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1", AllowPrivateAccess = "true"))
	float LeavesPlumpness = 0.5f;

public:
	ATerrainManager();

	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;
	
	float GetCubeLengthHalf() const { return CubeLengthHalf; }

private:
	bool UpdatedPosition();
	void UpdateTerrains();
	void AddChunk();
	void RemoveChunk();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnVoxel(FVector InVoxelLocation, int32 X, int32 Y);
};
