// Fill out your copyright notice in the Description page of Project Settings.


#include "Lootable.h"
#include "Components/StaticMeshComponent.h"

ALootable::ALootable()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	InitialLifeSpan = 60.0f;
}

void ALootable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Self rotate by Z axix
	AddActorLocalRotation(FRotator(0.0f, 1.0f, 0.0f));

	// If not close to the ground, keep falling
}



