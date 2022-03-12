// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class ALoot;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZIXUANCRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	static constexpr int32 MaxSize = 36;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ALoot*> Inventory;

public:	
	UInventoryComponent();

	void AddLoot(ALoot* Loot);

	bool IsFull() const { return Inventory.Num() >= MaxSize; }
};
