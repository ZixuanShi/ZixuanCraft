// Fill out your copyright notice in the Description page of Project Settings.


#include "ZixuanCraftInventoryButton.h"
#include "Components/TextBlock.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"
#include "ZixuanCraftMainGameWidget.h"

PRAGMA_DISABLE_OPTIMIZATION

void UZixuanCraftInventoryButton::Init(int32 NewIndex)
{
	Index = NewIndex;

	CountText = Cast<UTextBlock>(GetChildAt(0));
	OnPressed.AddDynamic(this, &UZixuanCraftInventoryButton::Select);
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
	WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void UZixuanCraftInventoryButton::Reset()
{
	WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 0.8f));
}

void UZixuanCraftInventoryButton::Select()
{
	if (AZixuanCraftCharacter* Character = Cast<AZixuanCraftCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()))
	{
		UZixuanCraftMainGameWidget* Widget = Character->GetWidget();

		// If this button serves all inventory panel, and the previous selected item is in the bottom inventory, perform a swap
		int32 SwapItemThreshold = Widget->GetBottomInventoryNum();
		if (Index >= SwapItemThreshold && 
			Widget->GetSelectIndex() != InvalidIndex)
		{
			UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
			PlayerInventoryComponent->SwapLoot(Index, Widget->GetSelectIndex());
			Widget->UpdateInventory(PlayerInventoryComponent->GetLootSlot(Index), Index);
			Widget->UpdateInventory(PlayerInventoryComponent->GetLootSlot(Widget->GetSelectIndex()), Widget->GetSelectIndex());
		}

		Widget->ResetSelectedInventory();
		Widget->SetSelectIndex(Index);
		Character->SetObjectInHand(Data.LootData.Type);
		Highlight();
	}
}

PRAGMA_ENABLE_OPTIMIZATION