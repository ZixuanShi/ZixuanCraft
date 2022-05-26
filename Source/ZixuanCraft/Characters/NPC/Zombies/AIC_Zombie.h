// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/AI/AIC_NPC.h"
#include "AIC_Zombie.generated.h"

/**
 * AI Controller for Zombies
 */
UCLASS()
class ZIXUANCRAFT_API AAIC_Zombie : public AAIC_NPC
{
	GENERATED_BODY()

public:
	virtual void InitBlackboardData() override final;

protected:
	virtual void OnSightPerceptionUpdatedImpl(AActor* Actor, const FAIStimulus& Stimulus) override final;
};
