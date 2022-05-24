// Fill out your copyright notice in the Description page of Project Settings.


#include "Debugging/Debugger.h"
#include "Characters/NPC/SpawnableCharacter.h"

#include "Kismet/GameplayStatics.h"

void ADebugger::BeginPlay()
{
	Super::BeginPlay();

	// Set TotalAgentCount to existing agents in the map already
	TArray<AActor*> ExistingNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnableCharacter::StaticClass(), ExistingNPCs);
	TotalAgentCount = ExistingNPCs.Num();
	for (AActor* NPC : ExistingNPCs)
	{
		NPC->OnEndPlay.AddDynamic(this, &ADebugger::OnNPCEndPlay);
	}

	// On NPC Spawn, increment TotalAgentCount
	OnActorSpawnDebugHandler.BindLambda([this](AActor* SpawnedActor)
		{
			if (SpawnedActor && SpawnedActor->IsA<ASpawnableCharacter>())
			{
				SpawnedActor->OnEndPlay.AddDynamic(this, &ADebugger::OnNPCEndPlay);
				++TotalAgentCount;
			}
		});
	GetWorld()->AddOnActorSpawnedHandler(OnActorSpawnDebugHandler);
}

void ADebugger::Cheat_SpawnNPC(UClass* NPCClass)
{

}

void ADebugger::Cheat_Heal()
{

}

void ADebugger::OnNPCEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	--TotalAgentCount;
}

