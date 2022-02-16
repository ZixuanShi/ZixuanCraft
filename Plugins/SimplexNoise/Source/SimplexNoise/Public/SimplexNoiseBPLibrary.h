// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimplexNoiseBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class SIMPLEXNOISE_API USimplexNoiseBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

private:
	static unsigned char perm[];
	static float grad(int hash, float x);
	static float grad(int hash, float x, float y);
	static float grad(int hash, float x, float y, float z);
	static float grad(int hash, float x, float y, float z, float t);

public:
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static void setNoiseSeed(const int32& newSeed);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise1D(float x);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise2D(float x, float y);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise3D(float x, float y, float z);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise4D(float x, float y, float z, float w);

	// Scaled by float value
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled1D(float x, float s);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled2D(float x, float y, float s);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled3D(float x, float y, float z, float s);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled4D(float x, float y, float z, float w, float s);

	// Return value in Range between two float numbers
	// Return Value is scaled by difference between rangeMin & rangeMax value
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange1D(float x, float rangeMin, float rangeMax);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange2D(float x, float y, float rangeMin, float rangeMax);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange3D(float x, float y, float z, float rangeMin, float rangeMax);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange4D(float x, float y, float z, float w, float rangeMin, float rangeMax);
};
