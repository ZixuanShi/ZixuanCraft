// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LootData.h"
#include "GameplayComponents/LootSlot.h"

#include "GameFramework/Actor.h"
#include "Math/Matrix.h"
#include "CraftingManager.generated.h"

/**
 * Responsible for the crafting logic in gameplay
 */
UCLASS()
class ZIXUANCRAFT_API ACraftingManager : public AActor
{
	GENERATED_BODY()
	
private:
	///** Hash Map used for looking up crafting data. Key uses a matrix, Value as the result */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//TMap<FMatrix, FLootData> CraftingMap;

};
