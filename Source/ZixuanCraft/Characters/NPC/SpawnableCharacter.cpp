// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableCharacter.h"
#include "Debugging/Debugger.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

ASpawnableCharacter::ASpawnableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->MaxStepHeight = 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = 255.0f;
}

float ASpawnableCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;

	UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	if (Health < 0.0f)
	{
		SetState(EAgentState::Dead);
		SetLifeSpan(DeathLifeSpan);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetNPCAnimInstance()->SetDead(true);
		if (OnDeadSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OnDeadSound, GetActorLocation(), 1.0f);
		}
	}
	else
	{
		if (bCanPlaySound && OnHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OnHitSound, GetActorLocation(), 1.0f);

			bCanPlaySound = false;
			GetWorld()->GetTimerManager().SetTimer(HitSoundTimerHandle, [this]() {	bCanPlaySound = true; }, OnHitSound->GetDuration(), false);
		}
		SetState(EAgentState::Engaged);
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ASpawnableCharacter::SetState(EAgentState InState)
{
	UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	Blackboard->SetValueAsEnum(FName("AgentState"), static_cast<uint8>(InState));
}

EAgentState ASpawnableCharacter::GetAgentState() const
{
	UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	return static_cast<EAgentState>(Blackboard->GetValueAsEnum(FName("AgentState")));
}
