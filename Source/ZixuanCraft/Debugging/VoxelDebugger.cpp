// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelDebugger.h"
#include "UObject/ConstructorHelpers.h"
PRAGMA_DISABLE_OPTIMIZATION

int32 AVoxelDebugger::GetIndex(int32 X, int32 Y, int32 Z) const
{
	int32 Index = X + (Y * CubeCountXY) + (Z * CubeCountXYSquared);
	return Index; 
}

FIntVector AVoxelDebugger::GetXYZFromLocation(FVector Location) const
{
	int32 X = Location.X / CubeLength;
	int32 Y = Location.Y / CubeLength;
	int32 Z = Location.Z / CubeLength;
	return FIntVector(X, Y, Z);
}

int32 AVoxelDebugger::GetIndexFromLocation(FVector Location) const
{
	FIntVector XYZ = GetXYZFromLocation(Location);
	return GetIndex(XYZ.X, XYZ.Y, XYZ.Z);
}

PRAGMA_ENABLE_OPTIMIZATION
