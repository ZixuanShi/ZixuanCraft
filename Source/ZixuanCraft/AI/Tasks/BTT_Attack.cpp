// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_Attack.h"
#include "GameplayComponents/AggressiveComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UAggressiveComponent* AggressiveComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UAggressiveComponent>();
	if (!AggressiveComponent)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	AggressiveComponent->Attack(TargetActor);
	return EBTNodeResult::Succeeded;
}
