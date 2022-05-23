#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAgentState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Engaged 	UMETA(DisplayName = "Engaged"),
	Dead		UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Runaway		UMETA(DisplayName = "Runaway"),
	Random 		UMETA(DisplayName = "Random"),
};