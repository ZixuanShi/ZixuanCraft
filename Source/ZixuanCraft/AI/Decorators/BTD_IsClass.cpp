// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_IsClass.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_IsClass::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Check if input TargetKey is inherited from ClassType
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	const UObject* TargetActor = BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName);
	return TargetActor && TargetActor->IsA(ClassType);
}
