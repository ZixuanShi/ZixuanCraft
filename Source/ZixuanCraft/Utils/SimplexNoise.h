#pragma once
#include "CoreMinimal.h"

class SimplexNoise
{
public:
	static void setNoiseSeed(const int32& newSeed);
	static float SimplexNoise2D(float x, float y);

private:
	static unsigned char perm[];
	static float grad(int hash, float x);
	static float grad(int hash, float x, float y);
	static float grad(int hash, float x, float y, float z);
	static float grad(int hash, float x, float y, float z, float t);
};