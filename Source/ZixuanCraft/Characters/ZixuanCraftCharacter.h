// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BaseCharacter.h"
#include "GameObjects/Terrain/TerrainVoxel.h"
#include "ZixuanCraftCharacter.generated.h"

class ATerrainManager;

UCLASS(config=Game)
class AZixuanCraftCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	static constexpr float OffsetHelper = 5.0f;		// Use this value to find the cube's location when interacting with voxel

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CubeInteractDistance = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ATerrainManager* TerrainManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLootData ObjectInHand;

public:
	virtual void BeginPlay() override final;

	/** Inputs */
	virtual void Attack() override final;
	void DestroyBlock();
	void PlaceBlock();
	void UseItem();

	void SetObjectInHand(const FLootData& Data) { ObjectInHand = Data; }
	 
private:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override final;
	// End of APawn interface

	/**
	 * When the player tries to Interact with terrain (Destroy/Place a cube), update a voxel according to player's action
	 * @param NewType				The new cube type to replace in terrain
	 * @param OffsetMultiplier		Used for accurately find the cube's index to update in the terrain. i.e. If the player tries to destroy a cube, we should grab the cube that's a little far away. If placing one, grab the cube location closer to the player
	 */
	void InteractVoxel(EObjectType NewType, float OffsetMultiplier);
};

