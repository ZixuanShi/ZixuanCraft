// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lootable.generated.h"

class UStaticMeshComponent;

UCLASS()
class ZIXUANCRAFT_API ALootable : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

public:	
	ALootable();

	virtual void Tick(float DeltaSeconds) override final;
};
