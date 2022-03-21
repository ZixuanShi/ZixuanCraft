// Fill out your copyright notice in the Description page of Project Settings.


#include "ZixuanCraftMainGameWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "ZixuanCraftInventoryButton.h"
#include "Characters/ZixuanCraftCharacter.h"

void UZixuanCraftMainGameWidget::NativeConstruct()
{
	// Blueprint class needs to add images to inventory_image
	Super::NativeConstruct();

	// For each inventory buttons, add event
	int32 InventoryButtonIndex = 0;
	for (UWidget* InventoryButtons : BottomInventoryItems_Panel->GetAllChildren())
	{
		UZixuanCraftInventoryButton* Button = Cast<UZixuanCraftInventoryButton>(InventoryButtons);
		if (Button)
		{
			Button->Init(InventoryButtonIndex);
			++InventoryButtonIndex;
		}
	}
	InventoryButtonIndex = 0;
	for (UWidget* InventoryButtons : AllInventoryItems_Panel->GetAllChildren())
	{
		Cast<UZixuanCraftInventoryButton>(InventoryButtons)->Init(InventoryButtonIndex);
		++InventoryButtonIndex;
	}

	// Initialize player's UI
	AZixuanCraftCharacter* Player = Cast<AZixuanCraftCharacter>(GetOwningPlayerPawn());
	Player->InitWidget(this);

	// Hide the all inventory panel
	SwitchInventory();

	// Hide mobile UI if not on mobile platforms
#if !PLATFORM_ANDROID && !PLATFORM_IOS
	if (Mobile_Panel)
	{
		Mobile_Panel->SetIsEnabled(false);
		Mobile_Panel->SetVisibility(ESlateVisibility::Hidden);
	}
	// If on mobile, bind events to the button clicks
#else
	Jump_Mobile_Button->OnPressed.AddDynamic(this, &UZixuanCraftMainGameWidget::OnJumpButtonPressed);
	Jump_Mobile_Button->OnReleased.AddDynamic(this, &UZixuanCraftMainGameWidget::OnJumpButtonReleased);
	DestroyAttack_Mobile_Button->OnPressed.AddDynamic(this, &UZixuanCraftMainGameWidget::OnDestoryAttackButtonPressed);
	PlaceUseItem_Mobile_Button->OnPressed.AddDynamic(this, &UZixuanCraftMainGameWidget::OnPlaceUseItemButtonPressed);
#endif
}

void UZixuanCraftMainGameWidget::ScrollInventory(bool bIsScrollingDown)
{
	const TArray<UWidget*>& BottomInventory = BottomInventoryItems_Panel->GetAllChildren();
	const bool bShowingAllInventory = AllInventoryItems_Panel->GetIsEnabled();

	if (bShowingAllInventory)
	{
		return;
	}

	int32 NewIndex = SelectIndex;
	if (bIsScrollingDown)
	{
		++NewIndex;
		NewIndex %= BottomInventory.Num();
	}
	else
	{
		--NewIndex;
		if (NewIndex < 0)
		{
			NewIndex = BottomInventory.Num() - 1;
		}
	}

	Cast<UZixuanCraftInventoryButton>(BottomInventory[NewIndex])->Select();
}

void UZixuanCraftMainGameWidget::SwitchInventory()
{
	const bool bShowing = AllInventoryItems_Panel->GetIsEnabled();
	if (bShowing)
	{
		AllInventoryItems_Panel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		AllInventoryItems_Panel->SetVisibility(ESlateVisibility::Visible);

	}
	GetOwningPlayer()->SetShowMouseCursor(!bShowing);
	AllInventoryItems_Panel->SetIsEnabled(!bShowing);
}

void UZixuanCraftMainGameWidget::UpdateInventory(const FLootSlot& InSlot, int32 Index)
{
	UZixuanCraftInventoryButton* InventoryButton = nullptr;

	// Bottom Inventory
	if (Index < BottomInventoryItems_Panel->GetAllChildren().Num())
	{
		InventoryButton = Cast<UZixuanCraftInventoryButton>(BottomInventoryItems_Panel->GetChildAt(Index));
		InventoryButton->Update(InSlot);
	}

	InventoryButton = Cast<UZixuanCraftInventoryButton>(AllInventoryItems_Panel->GetChildAt(Index));
	InventoryButton->Update(InSlot);
}

void UZixuanCraftMainGameWidget::ResetSelectedInventory()
{
	const TArray<UWidget*>& BottomInventory = BottomInventoryItems_Panel->GetAllChildren();
	Cast<UZixuanCraftInventoryButton>(BottomInventory[SelectIndex])->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 0.8f));
}

