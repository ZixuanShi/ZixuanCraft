#pragma once
#include "CoreMinimal.h"

/** Returns Index of cube in AllCubes by X Y Z index */
constexpr int32 GetIndexFromXYZ(int32 X, int32 Y, int32 Z, float Width, float Height)
{
	return X + (Y * Width) + (Z * Height);
}

/** Returns X Y Z index of cube in AllCubes by world location */
inline FIntVector GetXYZFromLocation(FVector Location, float Length)
{
	const int32 X = Location.X / Length;
	const int32 Y = Location.Y / Length;
	const int32 Z = Location.Z / Length;
	return FIntVector(X, Y, Z);
}

/** Returns index of cube in AllCubes by world location */
inline int32 GetIndexFromLocation(FVector Location, float Width, float Height, float Length)
{
	const FIntVector XYZ = GetXYZFromLocation(Location, Length);
	return GetIndexFromXYZ(XYZ.X, XYZ.Y, XYZ.Z, Width, Height);
}
