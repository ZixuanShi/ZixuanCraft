// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZixuanCraftCharacter.h"
#include "GameObjects/Terrain/TerrainManager.h"
#include "GameplayComponents/InventoryComponent.h"
#include "UI/ZixuanCraftWidgetBase.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AZixuanCraftCharacter::BeginPlay()
{
	Super::BeginPlay();

	TerrainManager = Cast<ATerrainManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATerrainManager::StaticClass()));
}

void AZixuanCraftCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind attack/destroy event
	PlayerInputComponent->BindAction("Destroy", IE_Repeat, this, &AZixuanCraftCharacter::DestroyBlock);

	// Bind use item/place block event
	PlayerInputComponent->BindAction("PlaceBlock", IE_Pressed, this, &AZixuanCraftCharacter::PlaceBlock);
	PlayerInputComponent->BindAction("UseItem", IE_Repeat, this, &AZixuanCraftCharacter::UseItem);
}

void AZixuanCraftCharacter::DestroyBlock()
{
	if (!Widget->IsDisplayingInventoryPanel())
	{
		InteractVoxel(EObjectType::Empty, -OffsetHelper);
	}
}
void AZixuanCraftCharacter::UseItem()
{
	// TODO: Use item like roasted chicken
	if (!Widget->IsDisplayingInventoryPanel())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Use Item");
	}
}
void AZixuanCraftCharacter::PlaceBlock()
{
	if (!Widget->IsDisplayingInventoryPanel())
	{
		if (ObjectInHand.Type < EObjectType::TreeLeaves &&	// Valid terrain cube
			ObjectInHand.Type != EObjectType::Empty)			// Not empty
		{
			InteractVoxel(ObjectInHand.Type, OffsetHelper);
		}
	}
}

void AZixuanCraftCharacter::Attack()
{
	// TODO replace by Minecraft style attack
	if (!Widget->IsDisplayingInventoryPanel())
	{
		Super::Attack();
	}
}

void AZixuanCraftCharacter::InteractVoxel(EObjectType NewType, float OffsetMultiplier)
{
	// Find the block to interact
	const APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	const FVector Start = PlayerCameraManager->GetCameraLocation();
	const FVector End = (PlayerCameraManager->GetActorForwardVector() * CubeInteractDistance) + Start;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

	// Do work if we hit a voxel to interact
	if (ATerrainVoxel* HitVoxel = Cast<ATerrainVoxel>(HitResult.Actor))
	{
		// If we are placing a cube
		const int32 SelectedIndex = Widget->IGetSelectIndex();
		if (NewType != EObjectType::Empty && SelectedIndex != InvalidIndex)
		{
			if (InventoryComponent->SubtractItem(SelectedIndex))
			{
				ObjectInHand.Reset();
			}
			Widget->SetButtonDataAt(InventoryComponent->GetLootSlot(SelectedIndex), SelectedIndex);
		}

		const FVector UnitDirection = (Start - HitResult.Location).GetSafeNormal() * OffsetMultiplier;
		const FVector RelativePostion = UnitDirection - HitVoxel->GetActorLocation();
		const FVector CubeLocation = RelativePostion + HitResult.Location + FVector(TerrainManager->GetCubeLengthHalf());
		HitVoxel->ModifyCube(CubeLocation, HitResult.Location, NewType);
	}
}

