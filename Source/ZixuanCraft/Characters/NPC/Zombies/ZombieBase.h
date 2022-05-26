// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "ZombieBase.generated.h"

/**
 * Base class for zombies
 */
UCLASS()
class ZIXUANCRAFT_API AZombieBase : public ASpawnableCharacter
{
	GENERATED_BODY()

public:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override final;
};
