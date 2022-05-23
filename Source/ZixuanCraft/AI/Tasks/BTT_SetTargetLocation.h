// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Utils/Enums.h"

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetTargetLocation.generated.h"

/**
 * Find and set target location based on the desired movement type
 */
UCLASS()
class ZIXUANCRAFT_API UBTT_SetTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	EMovementType MovementType = EMovementType::Random;

	UPROPERTY(EditAnywhere)
	float DistanceMin = 1000.0f;

	UPROPERTY(EditAnywhere)
	float DistanceMax = 5000.0f;

	/** Going to be needed for runaway movement */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	/** Calculated destination location will be stored to this value */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetLocationKey;

	/** The acceptable radius for destination point */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "MovementType == EMovementType::Runaway", EditConditionHides))
	float AcceptableRadius = 100.0f;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}

private:
	FVector GetRunawayLocation(UBehaviorTreeComponent& OwnerComp) const;
	FVector GetRandomLocation(UBehaviorTreeComponent& OwnerComp) const;
};
