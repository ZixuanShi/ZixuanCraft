// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIC_NPC.generated.h"

class UAIPerceptionComponent;

/**
 * 
 */
UCLASS()
class ZIXUANCRAFT_API AAIC_NPC : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComponent;

	/** Used for relaxing combat state from patrol state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RelaxTime = 3.0f;

	float RelaxTimer = 0.0f;

public: 
	AAIC_NPC();

	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaSeconds) override final;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

protected:
	void OnSightPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus);
};
