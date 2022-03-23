// Fill out your copyright notice in the Description page of Project Settings.


#include "ZixuanCraftInventoryButton.h"
#include "Components/TextBlock.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "GameplayComponents/InventoryComponent.h"
#include "ZixuanCraftMainGameWidget.h"

PRAGMA_DISABLE_OPTIMIZATION

static const FSlateColor NormalColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 0.8f));
static const FSlateColor HighlightColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

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
	WidgetStyle.Normal.TintColor = HighlightColor;
}

void UZixuanCraftInventoryButton::Reset()
{
	WidgetStyle.Normal.TintColor = NormalColor;
}

void UZixuanCraftInventoryButton::Select()
{
	if (AZixuanCraftCharacter* Character = Cast<AZixuanCraftCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()))
	{
		UZixuanCraftMainGameWidget* Widget = Character->GetWidget();
		int32 SwapItemThreshold = Widget->GetBottomInventoryNum();
		Widget->ResetInventory(Widget->GetSelectIndex());

		if (Index >= SwapItemThreshold && 
			Widget->GetSelectIndex() != InvalidIndex)
		{
			UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
			PlayerInventoryComponent->SwapLoot(Index, Widget->GetSelectIndex());
			Widget->UpdateInventory(PlayerInventoryComponent->GetLootSlot(Index), Index);
			Widget->UpdateInventory(PlayerInventoryComponent->GetLootSlot(Widget->GetSelectIndex()), Widget->GetSelectIndex());
			Widget->ResetInventory(Index);
			Widget->SetSelectIndex(InvalidIndex);
		}
		else
		{
			Widget->SetSelectIndex(Index);
		}
		Character->SetObjectInHand(Data.LootData);
		Highlight();
	}
}

PRAGMA_ENABLE_OPTIMIZATION