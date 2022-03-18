#pragma once
#include "CoreMinimal.h"

/** Maximum count for a single slot can hold. Used for the loot that can stack in the inventory */
static constexpr int32 MaxSlotCount = 64;

/** The order must perfectly match the ATerrainManager::Materials */
UENUM(BlueprintType)
enum class EObjectType : uint8
{
	Empty			UMETA(DisplayName = "Empty"),
	
	// Solid terrain cube. Should drop exactly one lootable cube when got destroyed
	Grass			UMETA(DisplayName = "Grass"),
	Dirt			UMETA(DisplayName = "Dirt"),
	Stone			UMETA(DisplayName = "Stone"),
	TreeTrunk		UMETA(DisplayName = "TreeTruck"),

	// Not solid, drop loot depending on the specific type
	TreeLeaves		UMETA(DisplayName = "TreeLeaves"),

	// Gameplay loots
	RoastedChicken	UMETA(DisplayName = "RoastedChicken"),

	// Non-Stackable starts here

	// Weapons
	Sword			UMETA(DisplayName = "Sword"),
};
