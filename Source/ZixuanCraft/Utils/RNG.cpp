// Fill out your copyright notice in the Description page of Project Settings.


#include "RNG.h"

FRNG::FRNG(uint64 Seed)
{
	Seeds[0] = Seed << 3 | Seed;
	Seeds[1] = Seed << 7 | Seed;
}

float FRNG::FRand(float Min, float Max)
{
	float RandomFloat = FRand();
	float Range = Max - Min;
	return RandomFloat * Range + Min;
}

float FRNG::FRand()
{
	float Result = 0.f;

	// mangling...
	uint64 Temp1 = Seeds[1];
	Seeds[0] ^= Seeds[0] << 23;
	uint64 Temp2 = Seeds[0] ^ Seeds[1] ^ (Seeds[0] >> 17) ^ (Seeds[1] >> 26);
	uint64 Temp3 = Temp2 + Seeds[1];

	Result = 1.0f / ((float)INT_MAX + 1.0f) * (0x7FFFFFFF & Temp3);

	// reassign seed
	Seeds[0] = Temp1;
	Seeds[1] = Temp2;

	return Result;
}

bool FRNG::RandBool()
{
	return static_cast<bool>(RandInRange(0, 1));
}

void FRNG::SetSeed(uint64 Seed)
{
	ensureMsgf(Seed != 0, TEXT("Seed should not be zero"));
	Seeds[0] = Seed << 3 | Seed;
	Seeds[1] = Seed << 7 | Seed;
}

FRNG& FRNG::Global()
{
	static FRNG GlobalRng;
	return GlobalRng;
}
