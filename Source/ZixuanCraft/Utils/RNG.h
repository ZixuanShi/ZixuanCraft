// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Random number generator using xorshift128+
 */
class ZIXUANCRAFT_API FRNG
{
private:
	uint64 Seeds[2];

public:
	/**
	 * Ctor
	 * @param Seed		Generally use unsigned(time(nullptr))
	 */
	FRNG(uint64 Seed = unsigned(time(nullptr)));

	/** 
	 * Returns a random number from min to max inclusive 
	 * @param Min	Minimum value inclusive
	 * @param Max	Maximum value inclusive
	 */
	template<typename Type = uint32> Type RandInRange(Type Min, Type Max);

	/**
	 * Returns a random integral value within max range
	 * @param Max	Largest possible value (inclusive)
	 */
	template<typename Type = uint32> Type RandInMax(Type Max);

	/**
	* Returns a random floating point from 0 to 1 inclusive
	* @param Min	Minimum value inclusive
	* @param Max	Maximum value inclusive
	*/
	float FRand(float Min, float Max);

	/**
	 * Returns a random floating point from 0 to 1 inclusive
	 */
	float FRand();

	/**
	 * Generate a random boolean
	 */
	bool RandBool();

	/**
	 * Update two seeds
	 * @param Seed	The seed as input
	 */
	void SetSeed(uint64 Seed);

	/**
	 * Returns a static global rng
	 */
	static FRNG& Global();

private:
	/**
	 * xorshift128+ algorithm from https://en.wikipedia.org/wiki/Xorshift
	 * @template Type	The result will be in this Type
	 */
	template<typename Type> Type XOrShift128Plus();
};

template<typename Type>
inline Type FRNG::XOrShift128Plus()
{
	static_assert(std::is_integral_v<Type>, "Type must be integral");

	uint64 T = Seeds[0];
	const uint64 S = Seeds[1];

	Seeds[0] = S;

	T ^= T << 23;		// a
	T ^= T >> 17;		// b -- Again, the shifts and the multipliers are tunable
	T ^= S ^ (S >> 26);	// c

	Seeds[1] = T;
	return Type(T + S);
}

template<typename Type>
inline Type FRNG::RandInRange(Type Min, Type Max)
{
	uint64 Mod = (uint64)Max - (uint64)Min + 1;
	return XOrShift128Plus<Type>() % Mod + Min;
}

template<typename Type>
inline Type FRNG::RandInMax(Type Max)
{
	return RandInRange<Type>(0, Max);
}