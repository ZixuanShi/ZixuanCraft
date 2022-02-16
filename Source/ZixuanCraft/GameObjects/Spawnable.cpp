// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawnable.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpawnable::ASpawnable()
	: AgentState{ EAgentState::Idle }
	, Health{ 30.0f }
	, DeathLifeSpan{ 2.0f }
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->MaxStepHeight = 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = 255.0f;
}

FSpawnableParam::FSpawnableParam()
	: ObjectType{ EObjectType::Zombie }
{		
}

FString FSpawnableParam::ToString() const
{
	return FString();
}

