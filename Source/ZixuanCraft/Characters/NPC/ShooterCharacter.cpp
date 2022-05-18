// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/ShooterCharacter.h"
#include "GameObjects/ZixuanCraftProjectile.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#define COLLISION_WEAPON		ECC_GameTraceChannel1

void AShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveForward(ForwardMovementValue);
	MoveRight(RightMovementValue);
}

void AShooterCharacter::RangeAttack(AActor* Target)
{
}

void AShooterCharacter::MoveForward(float Val)
{
}

void AShooterCharacter::MoveRight(float Val)
{
}

bool AShooterCharacter::CanHitTarget(const AActor* Target) const
{
	return false;
}

void AShooterCharacter::SetMovementInput(float InForwardMovementValue, float InRightMovementValue)
{
}

FVector AShooterCharacter::GetShootLocation() const
{
	return FVector();
}

FHitResult AShooterCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	return FHitResult();
}
