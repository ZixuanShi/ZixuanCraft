// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftButton.h"

#include "Components/TextBlock.h"

UZixuanCraftButton::UZixuanCraftButton()
{
	ClickMethod = EButtonClickMethod::Type::PreciseClick;
	OnClicked.AddDynamic(this, &UZixuanCraftButton::OnPressed);
}

void UZixuanCraftButton::Init(int32 NewIndex)
{
	Index = NewIndex;

	WidgetStyle.Normal.TintColor = NormalColor;
	WidgetStyle.Hovered.TintColor = HighlightColor;
	WidgetStyle.Pressed.TintColor = PressedColor;

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

void UZixuanCraftButton::OnPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Cyan, "Clicked Base button");
}
