// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Utils/Enums.h"

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetSpeed.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	MaxWalkSpeed		UMETA(DisplayName = "MaxWalkSpeed"),
};

/**
 * Set Pawn Move Speed
 */
UCLASS()
class ZIXUANCRAFT_API UBTT_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	ESpeedType SpeedType = ESpeedType::MaxWalkSpeed;

	UPROPERTY(EditAnywhere)
	float NewSpeed = 600.0f;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
	virtual void OnGameplayTaskActivated(UGameplayTask& GameplayTask) override final {}
};
