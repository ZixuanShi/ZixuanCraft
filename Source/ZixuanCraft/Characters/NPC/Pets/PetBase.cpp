// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Pets/PetBase.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APetBase::APetBase()
{
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));

	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
}
