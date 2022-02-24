// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainVoxel.h"
#include "ProceduralMeshComponent.h"
#include "SimplexNoiseBPLibrary.h"

static const int32 bTriangles[] = { 2, 1, 0, 0, 3, 2 };
static const FVector2D bUVs[] = { FVector2D(0.000000, 0.000000), FVector2D(0.000000, 1.000000), FVector2D(1.000000, 1.000000), FVector2D(1.000000, 0.000000) };
static const FVector bNormals0[] = { FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1) };
static const FVector bNormals1[] = { FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1) };
static const FVector bNormals2[] = { FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0) };
static const FVector bNormals3[] = { FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0) };
static const FVector bNormals4[] = { FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0) };
static const FVector bNormals5[] = { FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0) };
static const FVector bMask[] = { FVector(0.000000, 0.000000, 1.000000), FVector(0.000000, 0.000000, -1.000000), FVector(0.000000, 1.000000, 0.000000), FVector(0.000000, -1.000000, 0.000000), FVector(1.000000, 0.000000, 0.000000), FVector(-1.000000, 0.000000, 0.000000) };

ATerrainVoxel::ATerrainVoxel()
	: VoxelX{ 0 }
	, VoxelY{ 0 }
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATerrainVoxel::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Init members
	CubeLengthHalf = CubeLength / 2.0f;
	CubeCountXYSquared = CubeCountXY * CubeCountXY;
	TotalCubeCount = CubeCountXYSquared * CubeCountZ;
	NoiseResult.SetNum(CubeCountXYSquared);
	AllCubes.SetNum(TotalCubeCount);
	USimplexNoiseBPLibrary::setNoiseSeed(Seed);

	// Set voxel name in the editor
	FString Str = "Voxel_" + FString::FromInt(VoxelX) + "_" + FString::FromInt(VoxelY);
	FName Name = FName(*Str);
	ProceduralMeshComponent = NewObject<UProceduralMeshComponent>(this, Name);
	ProceduralMeshComponent->RegisterComponent();

	// Procedural component
	RootComponent = ProceduralMeshComponent;
	RootComponent->SetWorldTransform(Transform);

	GenerateChunk();
	UpdateMesh();
}

void ATerrainVoxel::GenerateChunk()
{
	CalculateNoise();

	for (int32 X = 0; X < CubeCountXY; ++X)
	{
		for (int32 Y = 0; Y < CubeCountXY; ++Y)
		{
			for (int32 Z = 0; Z < CubeCountZ; ++Z)
			{
				int32 Index = X + (Y * CubeCountXY) + (Z * CubeCountXYSquared);

				// Choose a type
				if (Z == GrassThreshold + 1 + NoiseResult[X + Y * CubeCountXY] && FRNG::Global().FRand() < SpawnObjectChance)
				{
					AllCubes[Index] = ECubeType::Spawnable;
				}
				else if (Z == GrassThreshold + NoiseResult[X + Y * CubeCountXY] && FRNG::Global().FRand() < GenerateTreeChance)
				{
					AllCubes[Index] = ECubeType::Tree;
				}
				else if (Z < GrassThreshold - StoneOffset + NoiseResult[X + Y * CubeCountXY])
				{
					AllCubes[Index] = ECubeType::Stone;
				}
				else if (Z < GrassThreshold - 1 + NoiseResult[X + Y * CubeCountXY])
				{
					AllCubes[Index] = ECubeType::Dirt;
				}
				else if (Z < GrassThreshold + NoiseResult[X + Y * CubeCountXY])
				{
					AllCubes[Index] = ECubeType::Grass;
				}
				else
				{
					AllCubes[Index] = ECubeType::Empty;
				}			
			}
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
	MeshSections.SetNum(Materials.Num());

	for (int32 X = 0; X < CubeCountXY; ++X)
	{
		for (int32 Y = 0; Y < CubeCountXY; ++Y)
		{
			for (int32 Z = 0; Z < CubeCountZ; ++Z)
			{
				UpdateSingleCube(X, Y, Z, MeshSections);
			}
		}
	}

	return MeshSections;
}

void ATerrainVoxel::UpdateSingleCube(int32 X, int32 Y, int32 Z, TArray<FMeshSection>& MeshSections)
{
	int32 Index = X + (CubeCountXY * Y) + (CubeCountXYSquared * Z);
	ECubeType CubeType = AllCubes[Index];

	if (CubeType == ECubeType::Tree)
	{
		FVector Location{ X * CubeLength, Y * CubeLength, Z * CubeLength };
		Spawn(ECubeType::Tree, Location + GetActorLocation());
	}
	else if (CubeType == ECubeType::Spawnable)
	{
		FVector Location{ X * CubeLength, Y * CubeLength, Z * CubeLength };
		Spawn(ECubeType::Spawnable, Location + GetActorLocation());
	}
	else if (CubeType != ECubeType::Empty)
	{
		HandleNonEmptyCube(X, Y, Z, static_cast<int32>(CubeType) - 1, MeshSections);
	}
	else
	{
		// Only empty cube types should get here
		check(CubeType == ECubeType::Empty);
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
	for (int32 i = 0; i < Materials.Num(); ++i)
	{
		ProceduralMeshComponent->SetMaterial(i, Materials[i]);
	}
}

void ATerrainVoxel::SetVoxel(FVector LocalPosition, ECubeType NewType)
{
	int32 X = LocalPosition.X / CubeLength;
	int32 Y = LocalPosition.Y / CubeLength;
	int32 Z = LocalPosition.Z / CubeLength;

	int32 Index = X + (Y * CubeCountXY) + (Z * CubeCountXYSquared);
	AllCubes[Index] = NewType;
	UpdateMesh();
}

void ATerrainVoxel::HandleNonEmptyCube(int32 X, int32 Y, int32 Z, const int32 MeshIndex, TArray<FMeshSection>& MeshSections)
{
	int32 Index = X + (CubeCountXY * Y) + (CubeCountXYSquared * Z);

	TArray<FVector>& Vertices = MeshSections[MeshIndex].Vertices;
	TArray<int32>& Triangles = MeshSections[MeshIndex].Triangles;
	TArray<FVector>& Normals = MeshSections[MeshIndex].Normals;
	TArray<FVector2D>& UVs = MeshSections[MeshIndex].UVs;
	TArray<FProcMeshTangent>& Tangents = MeshSections[MeshIndex].Tangents;
	TArray<FColor>& VertexColors = MeshSections[MeshIndex].VertexColors;
	int32 ElementID = MeshSections[MeshIndex].ElementID;

	// add faces, verticies, UVS and Normals
	int32 TriangleNum = 0;
	for (int32 CubeSideIndex = 0; CubeSideIndex < 6; CubeSideIndex++)
	{
		int32 NewIndex = Index + bMask[CubeSideIndex].X + (bMask[CubeSideIndex].Y * CubeCountXY) + (bMask[CubeSideIndex].Z * CubeCountXYSquared);

		bool bShouldAddVertices = false;
		if ((X + bMask[CubeSideIndex].X < CubeCountXY) && (X + bMask[CubeSideIndex].X >= 0) && (Y + bMask[CubeSideIndex].Y < CubeCountXY) && (Y + bMask[CubeSideIndex].Y >= 0))
		{
			if (NewIndex < AllCubes.Num() && NewIndex >= 0)
			{
				if (AllCubes[NewIndex] < ECubeType::Grass)
				{
					bShouldAddVertices = true;
				}
			}
		}
		else
		{
			bShouldAddVertices = true;
		}

		if (bShouldAddVertices)
		{
			Triangles.Emplace(bTriangles[0] + TriangleNum + ElementID);
			Triangles.Emplace(bTriangles[1] + TriangleNum + ElementID);
			Triangles.Emplace(bTriangles[2] + TriangleNum + ElementID);
			Triangles.Emplace(bTriangles[3] + TriangleNum + ElementID);
			Triangles.Emplace(bTriangles[4] + TriangleNum + ElementID);
			Triangles.Emplace(bTriangles[5] + TriangleNum + ElementID);
			TriangleNum += 4;	// add 4 verticies to next triangle

			AddVertices(Vertices, Normals, X, Y, Z, CubeSideIndex);

			UVs.Append(bUVs, UE_ARRAY_COUNT(bUVs));
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// R
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// G
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// B
			VertexColors.Emplace(255, 255, 255, CubeSideIndex);		// A
		}
	}

	MeshSections[MeshIndex].ElementID += TriangleNum;
}

void ATerrainVoxel::AddVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, int32 X, int32 Y, int32 Z, int32 CubeSideIndex)
{
	switch (CubeSideIndex)
	{
		case 0:
		{
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Normals.Append(bNormals0, UE_ARRAY_COUNT(bNormals0));
			break;
		}
		case 1:
		{
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Normals.Append(bNormals1, UE_ARRAY_COUNT(bNormals1));
			break;
		}
		case 2:
		{
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Normals.Append(bNormals2, UE_ARRAY_COUNT(bNormals2));
			break;
		}
		case 3:
		{
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Normals.Append(bNormals3, UE_ARRAY_COUNT(bNormals3));
			break;
		}
		case 4:
		{
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Normals.Append(bNormals4, UE_ARRAY_COUNT(bNormals4));
			break;
		}
		case 5:
		{
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), -CubeLengthHalf + (Z * CubeLength));
			Vertices.Emplace(-CubeLengthHalf + (X * CubeLength), -CubeLengthHalf + (Y * CubeLength), CubeLengthHalf + (Z * CubeLength));
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

	for (int32 Y = 0; Y < CubeCountXY; ++Y)
	{
		for (int32 X = 0; X < CubeCountXY; ++X)
		{
			// 1
			_X = (VoxelX * CubeCountXY + X) * 0.01f;
			_Y = (VoxelY * CubeCountXY + Y) * 0.01f;
			Result += USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * Weight;

			// 2
			_X = (VoxelX * CubeCountXY + X) * 0.01f;
			_Y = (VoxelY * CubeCountXY + Y) * 0.01f;
			Result += USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * Weight * 2.0f;

			// 3
			_X = (VoxelX * CubeCountXY + X) * 0.004f;
			_Y = (VoxelY * CubeCountXY + Y) * 0.004f;
			Result += USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * Weight * 4.0f;

			// 4
			_X = (VoxelX * CubeCountXY + X) * 0.05f;
			_Y = (VoxelY * CubeCountXY + Y) * 0.05f;
			Result += FMath::Clamp(USimplexNoiseBPLibrary::SimplexNoise2D(_X, _Y) * Weight, 0.0f, 5.0f);

			// Finalize
			NoiseResult[Y * CubeCountXY + X] = FMath::Floor(Result);
			Result = 0.0f;
		}
	}
}

bool ATerrainVoxel::InRange(int32 Value, int32 Range)
{
	return (Value >= 0 && Value <= Range);
}

