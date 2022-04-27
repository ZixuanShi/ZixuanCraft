// Fill out your copyright notice in the Description page of Project Settings.


#include "ZixuanCraftInventoryButton.h"
#include "Components/TextBlock.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"
#include "ZixuanCraftMainGameWidget.h"

PRAGMA_DISABLE_OPTIMIZATION

static const FSlateColor PressedColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
static const FSlateColor NormalColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 0.8f));
static const FSlateColor HighlightColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));	// On hover and selected

void UZixuanCraftInventoryButton::Init(int32 NewIndex)
{
	Index = NewIndex;

	WidgetStyle.Normal.TintColor = NormalColor;
	WidgetStyle.Hovered.TintColor = HighlightColor;
	WidgetStyle.Pressed.TintColor = PressedColor;

	CountText = Cast<UTextBlock>(GetChildAt(0));
	OnPressed.AddDynamic(this, &UZixuanCraftInventoryButton::OnSelected);
}

void UZixuanCraftInventoryButton::Update(const FLootSlot& InSlot)
{
	Data = InSlot;

	// Count text
	if (InSlot.Count > 0)
	{
		CountText->SetText(FText::AsNumber(InSlot.Count));
	}
	else
	{
		CountText->SetText(FText::FromString(""));
	}

	// Background Image
	WidgetStyle.Normal.SetResourceObject(InSlot.LootData.Icon);
	WidgetStyle.Hovered.SetResourceObject(InSlot.LootData.Icon);
	WidgetStyle.Pressed.SetResourceObject(InSlot.LootData.Icon);
}

void UZixuanCraftInventoryButton::Highlight()
{
	WidgetStyle.Normal.TintColor = HighlightColor;
}

void UZixuanCraftInventoryButton::Reset()
{
	WidgetStyle.Normal.TintColor = NormalColor;
}

void UZixuanCraftInventoryButton::OnSelected()
{
	// Data
	AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
	UZixuanCraftWidgetBase* Widget = Character->GetWidget();

	// Don't do anything if we don't have selected other button, and this button is empty
	if (Widget->IGetSelectIndex() == InvalidIndex && Data.Count == 0)
	{
		return;
	}

	// At this point, we are swapping two items in the inventory.
	const int32 SelectedIndex = Widget->IGetSelectIndex();
	Widget->ResetItemAt(SelectedIndex);		

	if (SelectedIndex != InvalidIndex &&
		Widget->IsDisplayingInventoryPanel())
	{
		UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
		PlayerInventoryComponent->SwapLoot(Index, SelectedIndex);
		Widget->IUpdateInventory(PlayerInventoryComponent->GetLootSlot(Index), Index);
		Widget->IUpdateInventory(PlayerInventoryComponent->GetLootSlot(SelectedIndex), SelectedIndex);
		Widget->ResetItemAt(Index);
		Widget->SetSelectIndex(InvalidIndex);
	}
	// We clicked an item for the first time, we want to select this item
	else
	{
		// If we are displaying inventory panel, make the item follows the mouse
		if (Widget->IsDisplayingInventoryPanel())
		{
			Widget->SetSelectedItem(Data);
			WidgetStyle.Normal.SetResourceObject(nullptr);
			WidgetStyle.Hovered.SetResourceObject(nullptr);
			WidgetStyle.Pressed.SetResourceObject(nullptr);
			CountText->SetText(FText::FromString(""));
		}

		Widget->SetSelectIndex(Index);
		Highlight();

		if (SelectedIndex < Widget->GetGameplayInventoryNum())
		{
			Character->SetObjectInHand(Data.LootData);
		}
	}
}

PRAGMA_ENABLE_OPTIMIZATION