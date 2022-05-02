// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftButton.h"
#include "UI/ZixuanCraftMainGameWidget.h"
#include "UI/ZixuanCraftInventoryButton.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

#include "Components/TextBlock.h"

UZixuanCraftButton::UZixuanCraftButton()
{
	ClickMethod = EButtonClickMethod::Type::DownAndUp;

	WidgetStyle.Normal.TintColor = NormalColor;
	WidgetStyle.Hovered.TintColor = HighlightColor;
	WidgetStyle.Pressed.TintColor = PressedColor;

	OnClicked.AddDynamic(this, &UZixuanCraftButton::OnPressed);
	OnHovered.AddDynamic(this, &UZixuanCraftButton::OnButtonHovered);
	OnUnhovered.AddDynamic(this, &UZixuanCraftButton::OnButtonUnhovered);
}

void UZixuanCraftButton::Init(int32 InWidgetIndex, int32 InPanelIndex, UZixuanCraftWidgetBase* InWidget, AZixuanCraftCharacter* InCharacter)
{
	WidgetIndex = InWidgetIndex;
	PanelIndex = InPanelIndex;
	Widget = InWidget;
	Character = InCharacter;
	CountText = Cast<UTextBlock>(GetChildAt(0));
}

void UZixuanCraftButton::SetData(const FLootSlot& InSlot)
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

void UZixuanCraftButton::Highlight()
{
	WidgetStyle.Normal.TintColor = HighlightColor;
}

void UZixuanCraftButton::Reset()
{
	WidgetStyle.Normal.TintColor = NormalColor;
}

void UZixuanCraftButton::OnLeftMouseFirstPressedImpl()
{
	// If we are displaying inventory panel, make the item follows the mouse
	if (Widget->IsDisplayingInventoryPanel())
	{
		// Clear the rendering data in the button if we are not running on mobile
#if !PLATFORM_ANDROID && !PLATFORM_IOS
		Widget->SetSelectedItemPanel(Data);
		WidgetStyle.Normal.SetResourceObject(nullptr);
		WidgetStyle.Hovered.SetResourceObject(nullptr);
		WidgetStyle.Pressed.SetResourceObject(nullptr);
		CountText->SetText(FText::FromString(""));
#endif
	}

	// Always set widget selected index and highlight it
	Widget->SetSelectIndex(WidgetIndex);
	Highlight();
}

void UZixuanCraftButton::OnLeftMouseSecondPressedImpl()
{		
	const int32 SelectedIndex = Widget->IGetSelectIndex();

	const int32 GameplayInventoryCount = Widget->GetGameplayInventoryNum();
	FLootSlot OtherLootSlot = Widget->GetSelectedSlotData();
	const FLootSlot ThisLootSlot = Data;

	// If they are the same type and count doesn't exceed max after merging, merge them
	if (ThisLootSlot.LootData.Type == OtherLootSlot.LootData.Type &&
		ThisLootSlot.Count + OtherLootSlot.Count <= MaxSlotCount)
	{
		OtherLootSlot.Count += ThisLootSlot.Count;
	}
	// Swap them if merging doesn't work
	else
	{
		// Set other button data
		Widget->SetButtonDataAt(ThisLootSlot, SelectedIndex);
		if (SelectedIndex >= GameplayInventoryCount &&
			SelectedIndex < GameplayInventoryCount * 2)
		{
			Widget->SetButtonDataAt(ThisLootSlot, SelectedIndex - GameplayInventoryCount);
		}
	}

	// Set this button data
	Widget->SetButtonDataAt(OtherLootSlot, WidgetIndex);
	if (WidgetIndex >= GameplayInventoryCount &&
		WidgetIndex < GameplayInventoryCount * 2)
	{
		Widget->SetButtonDataAt(OtherLootSlot, WidgetIndex - GameplayInventoryCount);
	}

	// Reset widget's selected index
	Widget->ResetItemAt(WidgetIndex);
	Widget->SetSelectIndex(InvalidIndex);
}

void UZixuanCraftButton::OnPressed()
{
	// Don't do anything if we don't have selected other button, and this button is empty
	if (Widget->IGetSelectIndex() == InvalidIndex && Data.Count == 0)
	{
		return;
	}

	// At this point, we are swapping two items of two buttons.
	const int32 SelectedIndex = Widget->IGetSelectIndex();
	Widget->ResetItemAt(SelectedIndex);

	// If we are swapping two items
	if (SelectedIndex != InvalidIndex &&
		Widget->IsDisplayingInventoryPanel())
	{
		OnLeftMouseSecondPressedImpl();
	}
	// We clicked an item for the first time, we want to select this item
	else
	{
		OnLeftMouseFirstPressedImpl();
	}
}

void UZixuanCraftButton::OnButtonHovered()
{
	Widget->SetLastHoveredButton(this);
}

void UZixuanCraftButton::OnButtonUnhovered()
{
	if (Widget->GetLastHoveredButton() == this)
	{
		Widget->SetLastHoveredButton(nullptr);
	}
}

void UZixuanCraftButton::OnRightMousePressed()
{
	// Data
	FLootSlot& SelectedSlot = Widget->GetSelectedSlotData();

	// Add 1 to the this button
	// If clicked button is empty, we need to copy selected slot data to it too
	if (Data.Count == 0)
	{
		Data.LootData = Widget->GetSelectedSlotData().LootData;
	}
	++Data.Count;
	SetData(Data);

	// Substract 1 count from selected item
	--SelectedSlot.Count;
	Widget->SetSelectedItemPanel(SelectedSlot);

	// Previous selected button was an inventory too
	const int32 SelectedIndex = Widget->IGetSelectIndex();
	UZixuanCraftButton* SelectedButton = Widget->GetButtonAt(SelectedIndex);
	UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	if (SelectedButton)
	{
		// If it rans out Count, reset it
		if (SelectedSlot.Count == 0)
		{
			if (SelectedButton->IsA<UZixuanCraftInventoryButton>())
			{
				InventoryComponent->ResetLootAt(Widget->ToBackpackIndex(SelectedIndex));
			}
			SelectedButton->Reset();
			SelectedButton->GetData().Reset();
			if (SelectedButton->GetPanelIndex() < Widget->GetGameplayInventoryNum())
			{
				Widget->GetButtonAt(SelectedButton->GetPanelIndex())->SetData(SelectedButton->GetData());
			}
			Widget->SetSelectIndex(InvalidIndex);
		}
		else
		{
			if (SelectedButton->IsA<UZixuanCraftInventoryButton>())
			{
				InventoryComponent->SetLootAt(Widget->GetSelectedSlotData(), Widget->ToBackpackIndex(SelectedIndex));
			}
		}
	}
}
