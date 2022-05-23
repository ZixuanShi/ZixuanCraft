// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_SetSpeed.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ACharacter* Character = OwnerComp.GetAIOwner()->GetCharacter();
	check(Character);

	switch (SpeedType)
	{
		case ESpeedType::MaxWalkSpeed:
			Character->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
			return EBTNodeResult::Succeeded;

		default:
			return EBTNodeResult::Failed;
	}
}
