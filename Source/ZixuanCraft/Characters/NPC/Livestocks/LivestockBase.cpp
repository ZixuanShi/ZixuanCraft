// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Livestocks/LivestockBase.h"
#include "Characters/NPC/AI/AIC_NPC.h"
#include "GameplayComponents/LootableComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

ALivestockBase::ALivestockBase()
{
	LootableComponent = CreateDefaultSubobject<ULootableComponent>(TEXT("LootableComponent"));
}

float ALivestockBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (Health > 0.0f)
	{
		SetState(EAgentState::Flee);
		SetProperty<UBlackboardKeyType_Object, UObject*>(FName("TargetActor"), DamageCauser);
		SetProperty<UBlackboardKeyType_Vector, FVector>(FName("TargetLocation"), DamageCauser->GetActorLocation());
	}

	return DamageTaken;
}
