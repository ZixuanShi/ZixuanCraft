// Fill out your copyright notice in the Description page of Project Settings.
// Loot.h
// Author: Zixuan Shi
// Base class for the game objects that can be collected by characters

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjects/ObjectEnums.h"
#include "GameObjects/Loot/LootData.h"
#include "Loot.generated.h"

/**
 * Base class for objects in the world that 
 * - Can be picked up by the player
 * - Can be dropped in gameplay from different resources
 * - Either stackable or not in the inventory 
 */
UCLASS()
class ZIXUANCRAFT_API ALoot : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FLootData LootData;

	/** Can be stacked in inventory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	bool bStackable = true;

public:	
	ALoot();

	virtual void Tick(float DeltaSeconds) override final;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetLootData(const FLootData& InLootData);

	bool IsStackable() const { return bStackable; }
	const FLootData& GetLootData() const { return LootData; }
	FLootData& GetLootData() { return LootData; }
	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
};
