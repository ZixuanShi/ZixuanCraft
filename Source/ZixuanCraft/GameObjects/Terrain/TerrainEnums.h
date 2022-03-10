#pragma once
#include "CoreMinimal.h"

/** The order must perfectly match the ATerrainManager::Materials */
UENUM(BlueprintType)
enum class ECubeType : uint8
{
	Empty		UMETA(DisplayName = "Empty"),
	Grass		UMETA(DisplayName = "Grass"),
	Dirt		UMETA(DisplayName = "Dirt"),
	Stone		UMETA(DisplayName = "Stone"),
	TreeTrunk	UMETA(DisplayName = "TreeTruck"),
	TreeLeaves	UMETA(DisplayName = "TreeLeaves"),
};
