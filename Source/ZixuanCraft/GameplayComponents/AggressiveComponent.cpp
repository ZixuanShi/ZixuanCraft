// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayComponents/AggressiveComponent.h"

void UAggressiveComponent::Attack(AActor* Target)
{
	// Attack cooldown
	if (!bCanAttack)
	{
		return;
	}
	bCanAttack = false;

	GetWorld()->GetTimerManager().SetTimer(AttackHandle, [this]()
		{
			bCanAttack = true;
		},
		AttackCooldown, false);

	// Actual damage happens here where owner actor overwrites
	OnAttack.Broadcast(Target);
}
