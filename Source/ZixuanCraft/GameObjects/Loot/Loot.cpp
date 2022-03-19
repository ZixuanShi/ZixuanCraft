// Fill out your copyright notice in the Description page of Project Settings.


#include "Loot.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

ALoot::ALoot()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 60.0f;

	LootData.MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	LootData.MeshComponent->SetCollisionProfileName("OverlapAllDynamic");
	LootData.MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ALoot::OnBeginOverlap);
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
	// if the overlapped actor is a Player
	if (AZixuanCraftCharacter* ZixuanCraftCharacter = Cast<AZixuanCraftCharacter>(OtherActor))
	{
		// Try adding this loot to his inventory
		UInventoryComponent* InventoryComponent = ZixuanCraftCharacter->GetInventoryComponent();
		if (InventoryComponent->TryAdd(this))
		{
			Destroy();
		}
	}
}



