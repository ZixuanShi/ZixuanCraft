// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootSlot.h"
#include "InventoryComponent.generated.h"

class ALoot;

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZIXUANCRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	/** Total maximum slots for an inventory */
	static constexpr int32 MaxSize = 36;

private:
	/** Buffer holding all the loots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FLootSlot> Inventory;

public:	
	/**
	 * Try to add a new loot into inventory
	 * @param Loot		The loot trying to add
	 * @return			true if succeeded, false if not
	 */
	bool TryAdd(ALoot* Loot);
};
