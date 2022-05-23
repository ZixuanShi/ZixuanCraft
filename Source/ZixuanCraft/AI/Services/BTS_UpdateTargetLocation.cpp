// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTS_UpdateTargetLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector TargetLocation;

	switch (MovementType)
	{
	case EMovementType::Runaway:		TargetLocation = GetRunawayLocation(OwnerComp);			break;
	case EMovementType::Random:			TargetLocation = GetRandomLocation(OwnerComp);			break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);
}

FVector UBTS_UpdateTargetLocation::GetRunawayLocation(UBehaviorTreeComponent& OwnerComp) const
{
	const FVector Origin = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const FVector RunawayFromLocation = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName))->GetActorLocation();
	const FVector Direction = (Origin - RunawayFromLocation).GetSafeNormal();
	const float Radius = FMath::FRandRange(DistanceMin, DistanceMax);
	const FVector Destination = Direction * FVector(Radius, Radius, 0.0f) + Origin;
	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation Result;
	NavSys->GetRandomReachablePointInRadius(Destination, AcceptableRadius, Result);
	return Result;
}

FVector UBTS_UpdateTargetLocation::GetRandomLocation(UBehaviorTreeComponent& OwnerComp) const
{
	const FVector Origin = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const float Radius = FMath::FRandRange(DistanceMin, DistanceMax);
	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation Result;
	NavSys->GetRandomReachablePointInRadius(Origin, Radius, Result);
	return Result;
}