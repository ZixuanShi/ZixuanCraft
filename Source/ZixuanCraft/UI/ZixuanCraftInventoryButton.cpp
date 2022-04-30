// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftInventoryButton.h"
#include "UI/ZixuanCraftMainGameWidget.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

#include "Components/TextBlock.h"

PRAGMA_DISABLE_OPTIMIZATION

void UZixuanCraftInventoryButton::OnFirstPressedImpl()
{	
	AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
	UZixuanCraftWidgetBase* Widget = Character->GetWidget();
	const int32 SelectedIndex = Widget->IGetSelectIndex();

	// If the selected button is in gameplay inventory, the character should hold that object in hand
	if (SelectedIndex < Widget->GetGameplayInventoryNum())
	{
		Character->SetObjectInHand(Data.LootData);
	}
	// If the selected button is in all items inventory, the character should hold nothing
	else
	{
		Character->SetObjectInHand(FLootData());
	}

	Widget->SetTempHackSelectIndex(WidgetIndex);
	Super::OnFirstPressedImpl();
}

void UZixuanCraftInventoryButton::OnSecondPressedImpl()
{		
	AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
	UZixuanCraftWidgetBase* Widget = Character->GetWidget();
	UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
	int32 SelectedIndex = Widget->IGetSelectIndex();

	// If two clicks happened between two inventory buttons. Perform a swap in inventory component as well
	if (SelectedIndex < Widget->GetTotalInventoryNum())
	{
		if (SelectedIndex > Widget->GetGameplayInventoryNum())
		{
			SelectedIndex = Widget->ToBackpackIndex(SelectedIndex);
		}

		PlayerInventoryComponent->SwapLoot(PanelIndex, SelectedIndex);
	}
	// If it's from crafting to inventory, we want to update this clicked inventory index and clear the previous one
	else
	{
		int32 TempHackIndex = Widget->IGetTempHackSelectIndex();
		if (TempHackIndex > Widget->GetGameplayInventoryNum())
		{
			TempHackIndex = Widget->ToBackpackIndex(TempHackIndex);
		}
		PlayerInventoryComponent->SwapLoot(PanelIndex, TempHackIndex);
	}

	Super::OnSecondPressedImpl();
}
PRAGMA_ENABLE_OPTIMIZATION
