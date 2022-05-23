// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/AI/AIC_NPC.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "Characters/Player/ZixuanCraftCharacter.h"
#include "GameplayComponents/AggressiveComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AAIC_NPC::AAIC_NPC()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight config
	UAISenseConfig_Sight* SightConfig = NewObject<UAISenseConfig_Sight>();
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// Bind function to AI perception
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIC_NPC::OnPerceptionUpdated);
}

void AAIC_NPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this) && GetPawn())
	{
		if (UAggressiveComponent* AggressiveComponent = GetPawn()->FindComponentByClass<UAggressiveComponent>())
		{
			GetBlackboardComponent()->SetValueAsFloat(FName("AttackRange"), AggressiveComponent->GetAttackRange());
		}
	}
}

void AAIC_NPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RelaxTimer -= DeltaSeconds;
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

void AAIC_NPC::OnSightPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	// Sense a player
	if (Actor->IsA<AZixuanCraftCharacter>())
	{
		// Seen
		if (Stimulus.WasSuccessfullySensed())
		{
			RelaxTimer = RelaxTime;
			GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
		}
		// Lost target, should move to last known player location
		else
		{
			GetBlackboardComponent()->ClearValue(FName("TargetActor"));
			GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), Actor->GetActorLocation());
		}

		GetBlackboardComponent()->SetValueAsBool(FName("bIsChasingPlayer"), true);
	}
}
