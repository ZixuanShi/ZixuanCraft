// Fill out your copyright notice in the Description page of Project Settings.


#include "ZixuanCraftInventoryButton.h"
#include "Components/TextBlock.h"

PRAGMA_DISABLE_OPTIMIZATION

void UZixuanCraftInventoryButton::Init(int32 NewIndex)
{
	Index = NewIndex;

	auto Children = GetAllChildren();
	CountText = Cast<UTextBlock>(GetChildAt(0));
	OnPressed.AddDynamic(this, &UZixuanCraftInventoryButton::Select);
}

void UZixuanCraftInventoryButton::Update(const FLootSlot& InSlot)
{
	// Count text
	if (InSlot.Count > 0)
	{
		CountText->SetText(FText::AsNumber(InSlot.Count));
	}

	// Background Image
	WidgetStyle.Normal.SetResourceObject(InSlot.LootData.Icon);
	WidgetStyle.Hovered.SetResourceObject(InSlot.LootData.Icon);
	WidgetStyle.Pressed.SetResourceObject(InSlot.LootData.Icon);
}

void UZixuanCraftInventoryButton::Select()
{
	// If there's an item in this inventory button.


	// Switch this item to the player's hand

}

PRAGMA_ENABLE_OPTIMIZATION