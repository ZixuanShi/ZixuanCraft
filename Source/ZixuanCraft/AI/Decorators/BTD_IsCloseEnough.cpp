// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_IsCloseEnough.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTD_IsCloseEnough::UBTD_IsCloseEnough()
{
	// accept only actors, floats, and vectors
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsCloseEnough, TargetKey), AActor::StaticClass());
	TargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsCloseEnough, TargetKey));
	CloseDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsCloseEnough, CloseDistanceKey));
	CloseDistanceKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsCloseEnough, CloseDistanceKey));

	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
	FlowAbortMode = EBTFlowAbortMode::Self;
}

bool UBTD_IsCloseEnough::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const FVector OriginLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	FVector TargetLocation;

	// Try to get key value as actor
	UObject* TargetActor = BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName);
	if (TargetActor)
	{
		TargetLocation = Cast<AActor>(TargetActor)->GetActorLocation();
	}
	else
	{
		TargetLocation = BlackboardComponent->GetValueAsVector(TargetKey.SelectedKeyName);
	}

	const float Distance = FVector::Distance(OriginLocation, TargetLocation);

	if (bUsingBlackboardKeyAsDistance)
	{
		return Distance < BlackboardComponent->GetValueAsFloat(CloseDistanceKey.SelectedKeyName);
	}
	else
	{
		return Distance < CloseDistance;
	}
}
