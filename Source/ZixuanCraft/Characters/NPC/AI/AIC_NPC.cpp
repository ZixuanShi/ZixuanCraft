// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/AI/AIC_NPC.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AAIC_NPC::AAIC_NPC()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight config
	SightConfig = NewObject<UAISenseConfig_Sight>();
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// Bind function to AI perception
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIC_NPC::OnPerceptionUpdated);
}

void AAIC_NPC::BeginPlay()
{
	Super::BeginPlay();

	InitBlackboardData();
}

void AAIC_NPC::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerceptionComponent->GetActorsPerception(Actor, Info);

		for (FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.Type.Name == FName("Default__AISense_Sight") &&
				Stimulus.Type.Index == 0)
			{
				OnSightPerceptionUpdated(Actor, Stimulus);
			}
		}
	}
}

void AAIC_NPC::OnSightPerceptionUpdated_Implementation(AActor* Actor, const FAIStimulus& Stimulus)
{
	OnSightPerceptionUpdatedImpl(Actor, Stimulus);
}
