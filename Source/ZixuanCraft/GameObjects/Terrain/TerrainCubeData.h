#pragma once
#include "CoreMinimal.h"

static const int32 bTriangles[] = { 2, 1, 0,
									0, 3, 2 };

static const FVector2D bUVs[] = { FVector2D(0.000000, 0.000000),
								  FVector2D(0.000000, 1.000000),
								  FVector2D(1.000000, 1.000000),
								  FVector2D(1.000000, 0.000000) };

static const FVector bNormals0[] = { FVector(0, 0, 1),
									 FVector(0, 0, 1),
									 FVector(0, 0, 1),
									 FVector(0, 0, 1) };

static const FVector bNormals1[] = { FVector(0, 0, -1),
									 FVector(0, 0, -1),
									 FVector(0, 0, -1),
									 FVector(0, 0, -1) };

static const FVector bNormals2[] = { FVector(0, 1, 0),
									 FVector(0, 1, 0),
									 FVector(0, 1, 0),
									 FVector(0, 1, 0) };

static const FVector bNormals3[] = { FVector(0, -1, 0),
									 FVector(0, -1, 0),
									 FVector(0, -1, 0),
									 FVector(0, -1, 0) };

static const FVector bNormals4[] = { FVector(1, 0, 0),
									 FVector(1, 0, 0),
									 FVector(1, 0, 0),
									 FVector(1, 0, 0) };

static const FVector bNormals5[] = { FVector(-1, 0, 0),
									 FVector(-1, 0, 0),
									 FVector(-1, 0, 0),
									 FVector(-1, 0, 0) };

static const FVector bMasks[] = { FVector(0.000000,  0.000000,  1.000000),	  // Top
								  FVector(0.000000,  0.000000, -1.000000),	  // Bottom
								  FVector(0.000000,  1.000000,  0.000000),	  // Front
								  FVector(0.000000, -1.000000,  0.000000),	  // Back
								  FVector(1.000000,  0.000000,  0.000000),	  // Right
								  FVector(-1.000000,  0.000000,  0.000000) }; // Left
