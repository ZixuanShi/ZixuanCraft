// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AggressiveComponent.generated.h"

class AZixuanCraftProjectile;

/**
 * Attach to SpawnableCharacter to make it able to attack other actors
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZIXUANCRAFT_API UAggressiveComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnAttackSignature, UAggressiveComponent, OnAttack, AActor*, TargetActor);

protected:
	/** Called whenever we attack, owner actor can bind behavior to this delegate */
	UPROPERTY(BlueprintAssignable)
	FOnAttackSignature OnAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 500.0f;

	FTimerHandle AttackHandle;

public:
	UFUNCTION(BlueprintCallable)
	void Attack(AActor* Target);

	float GetAttackRange() const { return AttackRange; }
};
