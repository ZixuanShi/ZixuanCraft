// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Zombies/ZombieBase.h"
#include "Characters/NPC/Zombies/AIC_Zombie.h"

float AZombieBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (Health > 0.0f)
	{
		SetState(EAgentState::Engaged);
	}

	return DamageTaken;
}
