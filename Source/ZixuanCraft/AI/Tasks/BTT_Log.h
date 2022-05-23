// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Log.generated.h"

/**
 * Logs a debug message to screen and output
 */
UCLASS()
class ZIXUANCRAFT_API UBTT_Log : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FString Message;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	float LogDuration = 5.0f;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	FColor ColorToDisplay = FColor::Cyan;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
};
