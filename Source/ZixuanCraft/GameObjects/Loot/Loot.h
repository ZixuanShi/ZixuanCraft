// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot.generated.h"

/**
 * Base class for objects in the world that 
 * - Can be picked up by the player
 * - Can be dropped by NPC
 * - Can be dropped by a terrain cube
 */
UCLASS()
class ZIXUANCRAFT_API ALoot : public AActor
{
	GENERATED_BODY()

protected:
	/** The mesh to render in game world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp = nullptr;

	/** The texture to draw in inventory UI Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconTexture = nullptr;

public:	
	ALoot();

	virtual void Tick(float DeltaSeconds) override final;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
