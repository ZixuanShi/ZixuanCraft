// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainVoxel.h"
#include "Characters/NPC/AI/AIC_NPC.h"
#include "Characters/NPC/NPCFactory.h"
#include "GameObjects/Terrain/TerrainManager.h"
#include "GameObjects/Terrain/TerrainCubeData.h"
#include "GameObjects/Loot/TerrainCubeLoot.h"
#include "Utils/Math.h"

#include "SimplexNoiseBPLibrary.h"

#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ATerrainVoxel::ATerrainVoxel()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	RootComponent = ProceduralMeshComponent;
}

void ATerrainVoxel::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// If in editor, terrainManager is not set, 
	if(!TerrainManager)
	{
		// Try to find it in the editor
		TerrainManager = Cast<ATerrainManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATerrainManager::StaticClass()));

		// if can't find, we use the default values
		if (!TerrainManager)
		{
			return;
		}
	}

	// Init members
	NoiseResult.SetNum(TerrainManager->CubeCountXYSquared);
	AllCubes.SetNum(TerrainManager->CubeCountXYSquared * TerrainManager->CubeCountZ);
	USimplexNoiseBPLibrary::setNoiseSeed(TerrainManager->Seed);
	RootComponent->SetWorldTransform(Transform);

	GenerateChunk();
	UpdateMesh();
}

void ATerrainVoxel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (ASpawnableCharacter* NPC : SpawnedObjects)
	{
		if (NPC->IsValidLowLevel())
		{
			GetWorld()->DestroyActor(NPC);
		}
	}

	SpawnedObjects.Empty();
}

void ATerrainVoxel::GenerateChunk()
{
	TArray<FIntVector> TreeRoots;

	CalculateNoise();

	// Set type for each cube. Spawn npc if so
	for (int32 X = 0; X < TerrainManager->CubeCountXY; ++X)
	{
		for (int32 Y = 0; Y < TerrainManager->CubeCountXY; ++Y)
		{
			for (int32 Z = 0; Z < TerrainManager->CubeCountZ; ++Z)
			{
				const int32 Index = GetIndexFromXYZ(X, Y, Z, TerrainManager->CubeCountXY, TerrainManager->CubeCountXYSquared);

				if (Z == TerrainManager->GrassThreshold + 1 + NoiseResult[X + Y * TerrainManager->CubeCountXY] && 
					FRNG::Global().FRand() < TerrainManager->SpawnObjectChance)
				{
					FVector Location{ X * TerrainManager->CubeLength, Y * TerrainManager->CubeLength, Z * TerrainManager->CubeLength };
					ASpawnableCharacter* SpawnedNPC = TerrainManager->NPCFactory->SpawnRandomNPC(Location + GetActorLocation());
					SpawnedObjects.Emplace(SpawnedNPC);
				}
				else if (Z == TerrainManager->GrassThreshold + NoiseResult[X + Y * TerrainManager->CubeCountXY] &&
						 FRNG::Global().FRand() < TerrainManager->SpawnTreeChance)
				{
					TreeRoots.Emplace(X, Y, Z);
				}
				else if (Z < TerrainManager->GrassThreshold - TerrainManager->StoneOffset + NoiseResult[X + Y * TerrainManager->CubeCountXY])
				{
					AllCubes[Index] = EObjectType::Stone;
				}
				else if (Z < TerrainManager->GrassThreshold - 1 + NoiseResult[X + Y * TerrainManager->CubeCountXY])
				{
					AllCubes[Index] = EObjectType::Dirt;
				}
				else if (Z < TerrainManager->GrassThreshold + NoiseResult[X + Y * TerrainManager->CubeCountXY])
				{
					AllCubes[Index] = EObjectType::Grass;
				}
				else
				{
					AllCubes[Index] = EObjectType::Empty;
				}			
			}
		}
	}

	// Trees
	for (FIntVector TreeRoot : TreeRoots)
	{
		const int32 TreeHeightMin   = static_cast<int32>(TerrainManager->TreeHeightRange.X);
		const int32 TreeHeightMax   = static_cast<int32>(TerrainManager->TreeHeightRange.Y);
		const int32 LeavesLegnthMin = static_cast<int32>(TerrainManager->LeavesLengthRange.X);
		const int32 LeavesLegnthMax = static_cast<int32>(TerrainManager->LeavesLengthRange.Y);

		const int32   TreeHeight      = FRNG::Global().RandInRange(TreeHeightMin, TreeHeightMax);
		const int32   LeavesWidth     = FRNG::Global().RandInRange(LeavesLegnthMin, LeavesLegnthMax);

		// Leaves 
		for (int32 X = -LeavesLegnthMax; X <= LeavesLegnthMax; ++X)
		{
			for (int32 Y = -LeavesLegnthMax; Y <= LeavesLegnthMax; ++Y)
			{
				for (int32 Z = -LeavesLegnthMax; Z <= LeavesLegnthMax; ++Z)
				{
					const bool bValidX = UKismetMathLibrary::InRange_IntInt(X + TreeRoot.X, 0, TerrainManager->CubeCountXY - 1);
					const bool bValidY = UKismetMathLibrary::InRange_IntInt(Y + TreeRoot.Y, 0, TerrainManager->CubeCountXY - 1);
					const bool bValidZ = UKismetMathLibrary::InRange_IntInt(Z + TreeRoot.Z + TreeHeight, 0, TerrainManager->CubeCountZ - 1);
					if (!bValidX || !bValidY || !bValidZ)
					{
						continue;	
					}
					
					// Make the leaves look more realistic, wipe out some of the leaves here
					const float RadiusSquard = FVector(X, Y, Z).SizeSquared();
					if (FRNG::Global().FRand() < TerrainManager->LeavesPlumpness ||		// Randomly wipe out some leaves
						RadiusSquard < (LeavesWidth / 2.0f))	// But don't wipe out the leaves close to the center
					{
						const int32 Index = GetIndexFromXYZ(TreeRoot.X + X, TreeRoot.Y + Y, TreeRoot.Z + Z + TreeHeight, TerrainManager->CubeCountXY, TerrainManager->CubeCountXYSquared);
						if (Index < AllCubes.Num())
						{
							AllCubes[Index] = EObjectType::TreeLeaves;
						}
					}
				}
			}
		}

		// Trunk
		for (int32 Z = 0; Z < TreeHeight; ++Z)
		{
			int32 Index = GetIndexFromXYZ(TreeRoot.X, TreeRoot.Y, TreeRoot.Z + Z, TerrainManager->CubeCountXY, TerrainManager->CubeCountXYSquared);
			AllCubes[Index] = EObjectType::TreeTrunk;
		}
	}
}

void ATerrainVoxel::UpdateMesh()
{
	const TArray<FMeshSection> MeshSections = GenerateMeshSections();
	RegenerateMesh(MeshSections);
	ApplyMaterials();
}

TArray<FMeshSection> ATerrainVoxel::GenerateMeshSections()
{
	TArray<FMeshSection> MeshSections;
	MeshSections.SetNum(TerrainManager->Materials.Num());

	for (int32 X = 0; X < TerrainManager->CubeCountXY; ++X)
	{
		for (int32 Y = 0; Y < TerrainManager->CubeCountXY; ++Y)
		{
			for (int32 Z = 0; Z < TerrainManager->CubeCountZ; ++Z)
			{
				UpdateSingleCube(X, Y, Z, MeshSections);
			}
		}
	}

	return MeshSections;
}

void ATerrainVoxel::UpdateSingleCube(int32 X, int32 Y, int32 Z, TArray<FMeshSection>& MeshSections)
{
	const int32 Index = GetIndexFromXYZ(X, Y, Z, TerrainManager->CubeCountXY, TerrainManager->CubeCountXYSquared);
	const EObjectType ObjectType = AllCubes[Index];

	if (ObjectType != EObjectType::Empty)
	{
		HandleNonEmptyCube(X, Y, Z, ObjectType, MeshSections);
	}
}

void ATerrainVoxel::RegenerateMesh(const TArray<FMeshSection>& MeshSections)
{
	ProceduralMeshComponent->ClearAllMeshSections();
	for (int32 i = 0; i < MeshSections.Num(); ++i)
	{
		if (MeshSections[i].Vertices.Num() > 0)
		{
			ProceduralMeshComponent->CreateMeshSection(i, MeshSections[i].Vertices, MeshSections[i].Triangles, MeshSections[i].Normals, MeshSections[i].UVs, MeshSections[i].VertexColors, MeshSections[i].Tangents, true);
		}
	}
}

void ATerrainVoxel::ApplyMaterials()
{
	for (int32 i = 0; i < TerrainManager->Materials.Num(); ++i)
	{
		ProceduralMeshComponent->SetMaterial(i, TerrainManager->Materials[i]);
	}
}

void ATerrainVoxel::ModifyCube(FVector CubeLocation, FVector SpawnLootLocation, EObjectType NewType)
{
	const int32 Index = GetIndexFromLocation(CubeLocation, TerrainManager->CubeCountXY, TerrainManager->CubeCountXYSquared, TerrainManager->CubeLength);
	EObjectType OriginalType = AllCubes[Index];
	AllCubes[Index] = NewType;
	UpdateMesh();

	// Create a loot
	if (OriginalType > EObjectType::WoodPlank || OriginalType == EObjectType::Empty)
	{
		return;
	}

	ATerrainCubeLoot* TerrainCubeLoot = GetWorld()->SpawnActor<ATerrainCubeLoot>(SpawnLootLocation, FRotator::ZeroRotator);
	TerrainCubeLoot->GetLootData().Type = OriginalType;
	TerrainCubeLoot->GetLootData().Icon = TerrainManager->Icons[static_cast<int32>(OriginalType)];
	TerrainCubeLoot->GetLootData().MeshComponent->SetMaterial(0, TerrainManager->GetMaterial(static_cast<int32>(OriginalType)));
}

void ATerrainVoxel::HandleNonEmptyCube(int32 X, int32 Y, int32 Z, const EObjectType ObjectType, TArray<FMeshSection>& MeshSections)
{
	const int32 Index = GetIndexFromXYZ(X, Y, Z, TerrainManager->CubeCountXY, TerrainManager->CubeCountXYSquared);
	const int32 CubeTypeMaterialIndex = static_cast<int32>(ObjectType);

	TArray<FVector>& Vertices = MeshSections[CubeTypeMaterialIndex].Vertices;
	TArray<int32>& Triangles = MeshSections[CubeTypeMaterialIndex].Triangles;
	TArray<FVector>& Normals = MeshSections[CubeTypeMaterialIndex].Normals;
	TArray<FVector2D>& UVs = MeshSections[CubeTypeMaterialIndex].UVs;
	TArray<FProcMeshTangent>& Tangents = MeshSections[CubeTypeMaterialIndex].Tangents;
	TArray<FColor>& VertexColors = MeshSections[CubeTypeMaterialIndex].VertexColors;
	const int32 ElementID = MeshSections[CubeTypeMaterialIndex].ElementID;

	// add faces, verticies, UVS and Normals
	int32 TriangleNum = 0;
	for (int32 CubeSideIndex = 0; CubeSideIndex < UE_ARRAY_COUNT(bMasks); ++CubeSideIndex)
	{
		const int32 MaskIndex = GetIndexFromXYZ(bMasks[CubeSideIndex].X, bMasks[CubeSideIndex].Y, bMasks[CubeSideIndex].Z, TerrainManager->CubeCountXY, TerrainManager->CubeCountXYSquared);
		const int32 NewIndex = Index + MaskIndex;
		const bool bValidX = UKismetMathLibrary::InRange_IntInt(X + bMasks[CubeSideIndex].X, 0, TerrainManager->CubeCountXY - 1);
		const bool bValidY = UKismetMathLibrary::InRange_IntInt(Y + bMasks[CubeSideIndex].Y, 0, TerrainManager->CubeCountXY - 1);
		bool bShouldAddVertices = false;

		if (ObjectType > EObjectType::WoodPlank)
		{
			bShouldAddVertices = true;
		}
		else if (bValidX && bValidY)
		{
			const bool bValidNewIndex = UKismetMathLibrary::InRange_IntInt(NewIndex, 0, AllCubes.Num() - 1);
			if (bValidNewIndex && 
				AllCubes[NewIndex] < EObjectType::Grass)
			{
				bShouldAddVertices = true;
			}
		}
		else
		{
			bShouldAddVertices = true;
		}

		if (bShouldAddVertices)
		{
			for (int32 Vertex : bTriangles)
			{
				Triangles.Emplace(Vertex + TriangleNum + ElementID);
			}
			TriangleNum += 4;	// add 4 verticies to next triangle

			AddVertices(Vertices, Normals, X, Y, Z, CubeSideIndex);

			UVs.Append(bUVs, UE_ARRAY_COUNT(bUVs));
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// R
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// G
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// B
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// A
		}
	}

	MeshSections[CubeTypeMaterialIndex].ElementID += TriangleNum;
}

void ATerrainVoxel::AddVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, int32 X, int32 Y, int32 Z, int32 CubeSideIndex)
{
	switch (CubeSideIndex)
	{
		case 0:
		{
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Normals.Append(bNormals0, UE_ARRAY_COUNT(bNormals0));
			break;
		}
		case 1:
		{
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Normals.Append(bNormals1, UE_ARRAY_COUNT(bNormals1));
			break;
		}
		case 2:
		{
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Normals.Append(bNormals2, UE_ARRAY_COUNT(bNormals2));
			break;
		}
		case 3:
		{
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Normals.Append(bNormals3, UE_ARRAY_COUNT(bNormals3));
			break;
		}
		case 4:
		{
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Normals.Append(bNormals4, UE_ARRAY_COUNT(bNormals4));
			break;
		}
		case 5:
		{
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Vertices.Emplace(-TerrainManager->CubeLengthHalf + (X * TerrainManager->CubeLength), -TerrainManager->CubeLengthHalf + (Y * TerrainManager->CubeLength), TerrainManager->CubeLengthHalf + (Z * TerrainManager->CubeLength));
			Normals.Append(bNormals5, UE_ARRAY_COUNT(bNormals5));
			break;
		}
	}
}

void ATerrainVoxel::CalculateNoise()
{
	float _X = 0.0f;
	float _Y = 0.0f;
	float Result = 0.0f;

	for (int32 Y = 0; Y < TerrainManager->CubeCountXY; ++Y)
	{
		for (int32 X = 0; X < TerrainManager->CubeCountXY; ++X)
		{
			// 1
			_X = (VoxelX * TerrainManager->CubeCountXY + X) * 0.01f;
			_Y = (VoxelY * TerrainManager->CubeCountXY + Y) * 0.01f;
			Result += USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * TerrainManager->Weight;

			// 2
			_X = (VoxelX * TerrainManager->CubeCountXY + X) * 0.01f;
			_Y = (VoxelY * TerrainManager->CubeCountXY + Y) * 0.01f;
			Result += USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * TerrainManager->Weight * 2.0f;

			// 3
			_X = (VoxelX * TerrainManager->CubeCountXY + X) * 0.004f;
			_Y = (VoxelY * TerrainManager->CubeCountXY + Y) * 0.004f;
			Result += USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * TerrainManager->Weight * 4.0f;

			// 4
			_X = (VoxelX * TerrainManager->CubeCountXY + X) * 0.05f;
			_Y = (VoxelY * TerrainManager->CubeCountXY + Y) * 0.05f;
			Result += FMath::Clamp(USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * TerrainManager->Weight, 0.0f, 5.0f);

			// Finalize
			NoiseResult[Y * TerrainManager->CubeCountXY + X] = FMath::Floor(Result);
			Result = 0.0f;
		}
	}
}
