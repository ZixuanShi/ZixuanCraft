// Fill out your copyright notice in the Description page of Project Settings.


#include "Loot.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

ALoot::ALoot()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.0f;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComp->SetCollisionProfileName("OverlapAllDynamic");
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ALoot::OnBeginOverlap);
	RootComponent = MeshComp;
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
		// If player's inventory isn't full, try adding this loot to his inventory
		UInventoryComponent* InventoryComponent = ZixuanCraftCharacter->FindComponentByClass<UInventoryComponent>();
		if (!InventoryComponent->IsFull())
		{
			InventoryComponent->AddLoot(this);
			MeshComp->SetVisibility(false);
			SetActorEnableCollision(false);
			SetActorTickEnabled(false);		// Stop falling or rotating
			GetWorldTimerManager().ClearTimer(TimerHandle_LifeSpanExpired);	// Don't destroy this loot when initial life span time is up anymore
		}
	}
}



