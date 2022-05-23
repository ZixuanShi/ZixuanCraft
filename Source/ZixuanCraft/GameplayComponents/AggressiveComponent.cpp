// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayComponents/AggressiveComponent.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "Characters/NPC/Animations/NPCAnimInstance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UAggressiveComponent::Attack(AActor* Target)
{
	// Attack cooldown
	if (!bCanAttack)
	{
		return;
	}

	bCanAttack = false;
	Cast<ASpawnableCharacter>(GetOwner())->GetNPCAnimInstance()->SetAttacking(true);

	GetWorld()->GetTimerManager().SetTimer(AttackHandle, [this]()
		{
			bCanAttack = true;
			Cast<ASpawnableCharacter>(GetOwner())->GetNPCAnimInstance()->SetAttacking(false);
		},
		AttackCooldown, false);

	// Actual damage happens here where owner actor overwrites
	OnAttack.Broadcast(Target);
}
