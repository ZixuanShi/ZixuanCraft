// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjects/ObjectEnums.h"
#include "Loot.generated.h"

/**
 * Base class for objects in the world that 
 * - Can be picked up by the player
 * - Can be dropped in gameplay from different resources
 * - Either stackable or not in the inventory 
 */
UCLASS()
class ZIXUANCRAFT_API ALoot : public AActor
{
	GENERATED_BODY()

protected:
	/** The mesh to render in game world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent = nullptr;

	/** The texture to draw in inventory UI Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	UTexture2D* IconTexture = nullptr;

	/** Can be stacked in inventory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	bool bStackable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EObjectType ObjectType = EObjectType::Dirt;

public:	
	ALoot();

	virtual void Tick(float DeltaSeconds) override final;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsStackable() const { return bStackable; }
	void SetType(EObjectType InCubeType) { ObjectType = InCubeType; }
	EObjectType GetType() const { return ObjectType; }
	UTexture2D* GetIcon() const { return IconTexture; }
	void SetIcon(UTexture2D* InIcon) { IconTexture = InIcon; }
	UStaticMeshComponent* GetMesh() const { return MeshComponent; }
};
