// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayComponents/LootSlot.h"

#include "Components/ActorComponent.h"
#include "LootableComponent.generated.h"

/**
 * Attach to Actor to make it drops loot on destroyed
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZIXUANCRAFT_API ULootableComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FLootSlot> LootsToDrop;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;
};
