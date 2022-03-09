// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lootable.generated.h"

class UStaticMeshComponent;

/**
 * Base class for objects in the world that 
 * - Can be picked up by the player
 * - Can be dropped by NPC
 */
UCLASS()
class ZIXUANCRAFT_API ALootable : public AActor
{
	GENERATED_BODY()

	/** The mesh to render in game world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

	/** The texture to draw in inventory UI Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconTexture = nullptr;

public:	
	ALootable();

	virtual void Tick(float DeltaSeconds) override final;
};
