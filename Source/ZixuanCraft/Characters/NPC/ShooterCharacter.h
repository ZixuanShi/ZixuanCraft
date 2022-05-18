// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Utils/Enums.h"

#include "Characters/NPC/SpawnableCharacter.h"
#include "ShooterCharacter.generated.h"

class AZixuanCraftProjectile;

/**
 * Shooter base class
 */
UCLASS()
class ZIXUANCRAFT_API AShooterCharacter : public ASpawnableCharacter
{
	GENERATED_BODY()

	static constexpr float WeaponPhysicsDamageMultiplier = 100000.0f;

protected:
	/** Gun mesh */
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/* This is when calculating the trace to determine what the weapon has hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponRange = 5000.0f;

	/* This is multiplied by the direction vector when the weapon trace hits something to apply velocity to the component that is hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDamage = 5.0f;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AZixuanCraftProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float ForwardMovementValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RightMovementValue = 0.0f;

public:
	virtual void Tick(float DeltaSeconds) override final;

	/** Fires a bullet to facing direction. Causes noise */
	virtual void RangeAttack() { RangeAttack(nullptr); }
	void RangeAttack(AActor* Target);

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/** Get if we can hit the target with a bullet at current transform */
	bool CanHitTarget(const AActor* Target) const;

	UFUNCTION(BlueprintCallable)
	void SetMovementInput(float InForwardMovementValue, float InRightMovementValue);

protected:
	/** Get shoot location */
	FVector GetShootLocation() const;

	/** Performs a trace between two points */
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;
};
