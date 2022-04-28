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

void UZixuanCraftButton::Init(int32 NewIndex)
{
	Index = NewIndex;
	CountText = Cast<UTextBlock>(GetChildAt(0));
}

void UZixuanCraftButton::Update(const FLootSlot& InSlot)
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

void UZixuanCraftButton::OnPressedImpl()
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

	// If we are swapping two items
	if (SelectedIndex != InvalidIndex &&
		Widget->IsDisplayingInventoryPanel())
	{
		AZixuanCraftCharacter* Character = GetOwningPlayer()->GetPawn<AZixuanCraftCharacter>();
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

			// Clear the rendering data in the button if we are not running on mobile
#if !PLATFORM_ANDROID && !PLATFORM_IOS
			WidgetStyle.Normal.SetResourceObject(nullptr);
			WidgetStyle.Hovered.SetResourceObject(nullptr);
			WidgetStyle.Pressed.SetResourceObject(nullptr);
			CountText->SetText(FText::FromString(""));
#endif
		}

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

		// Always set widget selected index and highlight it
		Widget->SetSelectIndex(Index);
		Highlight();
	}
}

void UZixuanCraftButton::OnPressed()
{
	OnPressedImpl();
}
