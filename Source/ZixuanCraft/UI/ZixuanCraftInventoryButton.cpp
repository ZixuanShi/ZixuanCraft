// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftInventoryButton.h"
#include "UI/ZixuanCraftMainGameWidget.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

#include "Components/TextBlock.h"

PRAGMA_DISABLE_OPTIMIZATION

void UZixuanCraftInventoryButton::OnPressedImpl()
{	
	Super::OnPressedImpl();

	AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
	UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Cyan, "Clicked Inventory button");
}

PRAGMA_ENABLE_OPTIMIZATION