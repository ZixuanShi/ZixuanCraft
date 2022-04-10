#pragma once
// MeshSection.h
// Author: Zixuan Shi
// Struct for ProceduralMeshComponent usage

#include "CoreMinimal.h"

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
