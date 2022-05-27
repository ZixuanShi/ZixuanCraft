// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/AI/AIC_NPC.h"
#include "AIC_Pet.generated.h"

/**
 * AI Controller for Pets
 */
UCLASS()
class ZIXUANCRAFT_API AAIC_Pet : public AAIC_NPC
{
	GENERATED_BODY()

public:
	virtual void InitBlackboardData() override final;

protected:
	virtual void OnSightPerceptionUpdatedImpl(AActor* Actor, const FAIStimulus& Stimulus) override final;
};
