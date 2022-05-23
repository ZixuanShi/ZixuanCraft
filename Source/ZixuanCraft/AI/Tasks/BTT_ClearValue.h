// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearValue.generated.h"

/**
 * Clear the input key value
 */
UCLASS()
class ZIXUANCRAFT_API UBTT_ClearValue : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector KeyToClear;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
};
