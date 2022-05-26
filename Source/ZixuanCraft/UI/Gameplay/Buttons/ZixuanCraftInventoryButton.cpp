// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/Buttons/ZixuanCraftInventoryButton.h"
#include "UI/Gameplay/Widgets/ZixuanCraftMainGameWidget.h"
#include "Characters/Player/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

#include "Components/TextBlock.h"

void UZixuanCraftInventoryButton::OnRightMousePressed()
{
	Super::OnRightMousePressed();

	// Data
	FLootSlot& SelectedSlot = Widget->GetSelectedSlotData();
	UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	const int32 SelectedIndex = Widget->IGetSelectIndex();

	// Set inventory component at panel index to this button's data
	InventoryComponent->SetLootAt(Data, PanelIndex);
	if (PanelIndex < Widget->GetGameplayInventoryNum())
	{
		Widget->GetButtonAt(PanelIndex)->SetData(Data);
	}
}

void UZixuanCraftInventoryButton::OnLeftMouseFirstPressedImpl()
{	
	UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
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

	if (Widget->IsDisplayingInventoryPanel())
	{
		// Remove loot from inventory
		PlayerInventoryComponent->ResetLootAt(PanelIndex);
	}

	Super::OnLeftMouseFirstPressedImpl();
	Widget->SetLastSelectedInventoryIndex(WidgetIndex);
}

void UZixuanCraftInventoryButton::OnLeftMouseSecondPressedImpl()
{		
	UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
	int32 SelectedIndex = Widget->IGetSelectIndex();

	if (SelectedIndex < Widget->GetTotalInventoryNum())
	{
		if (SelectedIndex > Widget->GetGameplayInventoryNum())
		{
			SelectedIndex = Widget->ToBackpackIndex(SelectedIndex);
		}
		PlayerInventoryComponent->SetLootAt(Data, SelectedIndex);
		PlayerInventoryComponent->SetLootAt(Widget->GetSelectedSlotData(), PanelIndex);
	}
	// If it's from a crafting button to here
	else if (SelectedIndex >= Widget->GetTotalInventoryNum())
	{
		PlayerInventoryComponent->SetLootAt(Widget->GetSelectedSlotData(), PanelIndex);
	}

	Super::OnLeftMouseSecondPressedImpl();
}
