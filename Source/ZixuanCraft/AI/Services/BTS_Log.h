// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Log.generated.h"

/**
 * 
 */
UCLASS()
class ZIXUANCRAFT_API UBTS_Log : public UBTService
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
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
	virtual void OnGameplayTaskDeactivated(UGameplayTask& GameplayTask) override final {}
};
