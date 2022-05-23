// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsCloseEnough.generated.h"

/**
 * Check if input key's location is close enough or not
 */
UCLASS()
class ZIXUANCRAFT_API UBTD_IsCloseEnough : public UBTDecorator
{
	GENERATED_BODY()

private:
	/** This can be an actor or a FVector value */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	bool bUsingBlackboardKeyAsDistance = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUsingBlackboardKeyAsDistance", EditConditionHides))
	FBlackboardKeySelector CloseDistanceKey;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUsingBlackboardKeyAsDistance", EditConditionHides))
	float CloseDistance = 500.0f;

public:
	UBTD_IsCloseEnough();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
	virtual void OnGameplayTaskDeactivated(UGameplayTask& GameplayTask) override final {}
};
