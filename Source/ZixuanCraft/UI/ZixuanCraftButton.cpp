// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftButton.h"
#include "UI/ZixuanCraftMainGameWidget.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"

#include "Components/TextBlock.h"

UZixuanCraftButton::UZixuanCraftButton()
{
	ClickMethod = EButtonClickMethod::Type::PreciseClick;
	WidgetStyle.Normal.TintColor = NormalColor;
	WidgetStyle.Hovered.TintColor = HighlightColor;
	WidgetStyle.Pressed.TintColor = PressedColor;
	OnClicked.AddDynamic(this, &UZixuanCraftButton::OnPressed);
}

void UZixuanCraftButton::Init(int32 InWidgetIndex, int32 InPanelIndex)
{
	WidgetIndex = InWidgetIndex;
	PanelIndex = InPanelIndex;
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

void UZixuanCraftButton::OnFirstPressedImpl()
{
	AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
	UZixuanCraftWidgetBase* Widget = Character->GetWidget();

	// If we are displaying inventory panel, make the item follows the mouse
	if (Widget->IsDisplayingInventoryPanel())
	{
		Widget->SetSelectedItemPanel(Data);

		// Clear the rendering data in the button if we are not running on mobile
#if !PLATFORM_ANDROID && !PLATFORM_IOS
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

void UZixuanCraftButton::OnSecondPressedImpl()
{		
	AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
	UZixuanCraftWidgetBase* Widget = Character->GetWidget();
	const int32 SelectedIndex = Widget->IGetSelectIndex();

	const int32 GameplayInventoryCount = Widget->GetGameplayInventoryNum();
	const FLootSlot OtherLootSlot = Widget->GetButtonAt(SelectedIndex)->Data;
	const FLootSlot ThisLootSlot = Data;

	// Set this button data
	Widget->SetButtonDataAt(OtherLootSlot, WidgetIndex);
	if (WidgetIndex >= GameplayInventoryCount && 
		WidgetIndex < GameplayInventoryCount * 2)
	{
		Widget->SetButtonDataAt(OtherLootSlot, WidgetIndex - GameplayInventoryCount);
	}

	// Set other button data
	Widget->SetButtonDataAt(ThisLootSlot, SelectedIndex);
	if (SelectedIndex >= GameplayInventoryCount &&
		SelectedIndex < GameplayInventoryCount * 2)
	{
		Widget->SetButtonDataAt(ThisLootSlot, SelectedIndex - GameplayInventoryCount);
	}

	// Reset widget's selected index
	Widget->ResetItemAt(WidgetIndex);
	Widget->SetSelectIndex(InvalidIndex);
}

void UZixuanCraftButton::OnPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Cyan, GetName());

	// Data
	AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
	UZixuanCraftWidgetBase* Widget = Character->GetWidget();

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
		OnSecondPressedImpl();
	}
	// We clicked an item for the first time, we want to select this item
	else
	{
		OnFirstPressedImpl();
	}
}
