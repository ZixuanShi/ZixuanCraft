// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "LivestockBase.generated.h"

class ULootableComponent;

/**
 * Base class for livestocks
 */
UCLASS()
class ZIXUANCRAFT_API ALivestockBase : public ASpawnableCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ULootableComponent* LootableComponent;

public:
	ALivestockBase();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override final;
};
