// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/TypeDefs.h"
#include "Utils/RNG.h"
#include "TerrainVoxel.generated.h"

class UStaticMeshComponent;
class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UProceduralMeshComponent;
struct FProcMeshTangent;

struct FMeshSection
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;
	int32 ElementID = 0;
};

UENUM(BlueprintType)
enum class ECubeType : uint8
{
	Empty		UMETA(DisplayName = "Empty"),
	Grass		UMETA(DisplayName = "Grass"),
	Dirt		UMETA(DisplayName = "Dirt"),
	Stone		UMETA(DisplayName = "Stone"),
	Tree		UMETA(DisplayName = "Tree"),
	Spawnable	UMETA(DisplayName = "Spawnable"),
};

/**
* Voxel holding the terrain cubes
*/
UCLASS()
class ZIXUANCRAFT_API ATerrainVoxel : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 VoxelX;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 VoxelY;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 CubeCountXY;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 CubeCountZ;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	float CubeLength;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 GrassThreshold;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 StoneOffset;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	float GenerateTreeChance;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	float Weight;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	float SpawnObjectChance;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 Seed;

	UPROPERTY(BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float CubeLengthHalf;

	UPROPERTY(BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	int32 TotalCubeCount;

	UPROPERTY(BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountXYSquared;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> SpawnedObjects;

	TArray<ECubeType> AllCubes;
	TArray<int32> NoiseResult;	// Temp hack

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
	void HandleNonEmptyCube(int32 X, int32 Y, int32 Z, const int32 MeshIndex, TArray<FMeshSection>& MeshSections);
	void AddVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, int32 X, int32 Y, int32 Z, int32 CubeSideIndex);

	/** Utils */
	bool InRange(int32 Value, int32 Range);

protected:
	/** Calculate perlin noise value for terrain generation */
	UFUNCTION(BlueprintCallable)
	void CalculateNoise();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Spawn(ECubeType Type, FVector Location);
};
