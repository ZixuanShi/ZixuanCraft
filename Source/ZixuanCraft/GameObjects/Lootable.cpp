// Fill out your copyright notice in the Description page of Project Settings.


#include "Lootable.h"
#include "Components/StaticMeshComponent.h"

ALootable::ALootable()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
}



