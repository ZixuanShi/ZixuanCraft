// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIC_NPC.generated.h"

class UAISenseConfig_Sight;
class UAIPerceptionComponent;

/**
 * Base AI Controller for NPC
 */
UCLASS()
class ZIXUANCRAFT_API AAIC_NPC : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComponent;

	UAISenseConfig_Sight* SightConfig = nullptr;

public: 
	AAIC_NPC();

	virtual void BeginPlay() override final;

	virtual void InitBlackboardData() {}

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UAISenseConfig_Sight* GetAIPerceptionSightConfig() const { return SightConfig; }

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnSightPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus);

	virtual void OnSightPerceptionUpdatedImpl(AActor* Actor, const FAIStimulus& Stimulus) {}
};
