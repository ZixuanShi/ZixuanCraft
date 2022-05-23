// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_ClearValue.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_ClearValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->ClearValue(KeyToClear.SelectedKeyName);
	return EBTNodeResult::Succeeded;
}
