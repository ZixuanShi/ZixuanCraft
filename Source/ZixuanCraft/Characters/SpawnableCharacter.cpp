// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpawnableCharacter::ASpawnableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->MaxStepHeight = 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = 255.0f;
}
