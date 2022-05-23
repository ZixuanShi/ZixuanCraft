// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * Attack a target actor
 */
UCLASS()
class ZIXUANCRAFT_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()

private:
	/** The target to attack */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
};
