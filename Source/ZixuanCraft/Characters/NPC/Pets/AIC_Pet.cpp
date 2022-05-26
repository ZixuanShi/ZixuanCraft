// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Pets/AIC_Pet.h"

#include "BehaviorTree/BlackboardComponent.h"

void AAIC_Pet::InitBlackboardData()
{
	if (IsValid(this) && GetPawn())
	{
		GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), GetWorld()->GetFirstPlayerController()->GetPawn());
	}
}