// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ASpawnableCharacter::ASpawnableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->MaxStepHeight = 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = 255.0f;
}

float ASpawnableCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;

	if (Health < 0.0f)
	{
		AgentState = EAgentState::Dead;
		SetLifeSpan(DeathLifeSpan);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		AgentState = EAgentState::Engaged;
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}
