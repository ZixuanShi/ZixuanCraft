// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsClass.generated.h"

/**
 * Check if input target shooter actor is the type
 */
UCLASS()
class ZIXUANCRAFT_API UBTD_IsClass : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	UClass* ClassType;

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
	virtual void OnGameplayTaskDeactivated(UGameplayTask& GameplayTask) override final {}
};
