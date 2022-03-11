#pragma once
#include "CoreMinimal.h"

/** The order must perfectly match the ATerrainManager::Materials */
UENUM(BlueprintType)
enum class ECubeType : uint8
{
	Empty		UMETA(DisplayName = "Empty"),
	
	// Solid terrain cube. Should drop exactly one lootable cube when got destroyed
	Grass		UMETA(DisplayName = "Grass"),
	Dirt		UMETA(DisplayName = "Dirt"),
	Stone		UMETA(DisplayName = "Stone"),
	TreeTrunk	UMETA(DisplayName = "TreeTruck"),

	// Not solid, drop loot depending on the specific type
	TreeLeaves	UMETA(DisplayName = "TreeLeaves"),
};
