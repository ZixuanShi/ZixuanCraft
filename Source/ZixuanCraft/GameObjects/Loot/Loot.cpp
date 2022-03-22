// Fill out your copyright notice in the Description page of Project Settings.


#include "Loot.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

ALoot::ALoot()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 60.0f;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	LootData.MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	LootData.MeshComponent->SetCollisionProfileName("OverlapAllDynamic");
	LootData.MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ALoot::OnBeginOverlap);
	LootData.MeshComponent->OnComponentEndOverlap.AddDynamic(this, &ALoot::OnOverlap);
	RootComponent = LootData.MeshComponent;
}

void ALoot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Self rotate by Z axix
	AddActorLocalRotation(FRotator(0.0f, 1.0f, 0.0f));

	// If not close to the ground, keep falling
}

void ALoot::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ALoot::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// if the overlapped actor is a Player
	AZixuanCraftCharacter* ZixuanCraftCharacter = Cast<AZixuanCraftCharacter>(OtherActor);
	if (ZixuanCraftCharacter && !IsPendingKillPending())
	{
		// Try adding this loot to his inventory
		UInventoryComponent* InventoryComponent = ZixuanCraftCharacter->GetInventoryComponent();
		int32 Index = InventoryComponent->TryAdd(this);
		if (Index != InvalidIndex)
		{
			ZixuanCraftCharacter->UpdateInventoryUI(Index);
			Destroy();
		}
	}
}
