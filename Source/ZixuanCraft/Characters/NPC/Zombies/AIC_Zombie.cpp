// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Zombies/AIC_Zombie.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "Characters/Player/ZixuanCraftCharacter.h"
#include "GameplayComponents/AggressiveComponent.h"
#include "Utils/Enums.h"

#include "BehaviorTree/BlackboardComponent.h"

void AAIC_Zombie::InitBlackboardData()
{
	if (IsValid(this) && GetPawn())
	{
		if (UAggressiveComponent* AggressiveComponent = GetPawn()->FindComponentByClass<UAggressiveComponent>())
		{
			GetBlackboardComponent()->SetValueAsFloat(FName("AttackRange"), AggressiveComponent->GetAttackRange());
		}
	}
}

void AAIC_Zombie::OnSightPerceptionUpdatedImpl(AActor* Actor, const FAIStimulus& Stimulus)
{	
	// Sense a player
	if (Actor->IsA<AZixuanCraftCharacter>())
	{
		// Seen
		if (Stimulus.WasSuccessfullySensed())
		{
			RelaxTimer = RelaxTime;
			GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
			GetPawn<ASpawnableCharacter>()->SetState(EAgentState::Engaged);
		}
		// Lost target, should move to last known player location
		else
		{
			GetBlackboardComponent()->ClearValue(FName("TargetActor"));
			GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), Actor->GetActorLocation());
			GetPawn<ASpawnableCharacter>()->SetState(EAgentState::Searching);
		}

		GetBlackboardComponent()->SetValueAsBool(FName("bIsChasingPlayer"), true);
	}
}

