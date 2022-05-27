// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Pets/AIC_Pet.h"
#include "Characters/NPC/Zombies/ZombieBase.h"
#include "GameplayComponents/AggressiveComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

void AAIC_Pet::InitBlackboardData()
{
	if (IsValid(this) && GetPawn())
	{
		if (UAggressiveComponent* AggressiveComponent = GetPawn()->FindComponentByClass<UAggressiveComponent>())
		{
			GetBlackboardComponent()->SetValueAsFloat(FName("AttackRange"), AggressiveComponent->GetAttackRange());
		}

		GetBlackboardComponent()->SetValueAsObject(FName("Owner"), GetWorld()->GetFirstPlayerController()->GetPawn());
	}
}

void AAIC_Pet::OnSightPerceptionUpdatedImpl(AActor* Actor, const FAIStimulus& Stimulus)
{
	// Sense a zombie
	if (Actor->IsA<AZombieBase>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
			GetPawn<ASpawnableCharacter>()->SetState(EAgentState::Engaged);
		}
		else
		{
			GetBlackboardComponent()->ClearValue(FName("TargetActor"));
		}
	}

	if (!GetBlackboardComponent()->GetValueAsObject(FName("TargetActor")))
	{
		GetPawn<ASpawnableCharacter>()->SetState(EAgentState::Idle);
	}
}
