// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Utils/Enums.h"

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetAgentState.generated.h"

/**
 * Set agent's state
 */
UCLASS()
class ZIXUANCRAFT_API UBTT_SetAgentState : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	EAgentState NewState = EAgentState::Idle;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
};
