// Fill out your copyright notice in the Description page of Project Settings.


#include "ZixuanCraftMainGameWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "ZixuanCraftInventoryButton.h"
#include "Characters/ZixuanCraftCharacter.h"

void UZixuanCraftMainGameWidget::NativeConstruct()
{
	// Blueprint class needs to add images to inventory_image
	Super::NativeConstruct();

	// For each inventory buttons, add event
	int32 InventoryButtonIndex = 0;
	const TArray<UWidget*>& BottomInventory = BottomInventoryItems_Panel->GetAllChildren();
	for (UWidget* InventoryButtons : BottomInventory)
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
	ToggleInventory();

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
	ToggleInventory_Mobile_Button->OnPressed.AddDynamic(this, &UZixuanCraftMainGameWidget::SwitchInventory);
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

void UZixuanCraftMainGameWidget::ToggleInventory()
{
	const bool bShowing = AllInventoryItems_Panel->GetIsEnabled();
	if (bShowing)
	{
		AllInventoryItems_Panel->SetVisibility(ESlateVisibility::Hidden);
		BottomInventoryItems_Panel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AllInventoryItems_Panel->SetVisibility(ESlateVisibility::Visible);
		BottomInventoryItems_Panel->SetVisibility(ESlateVisibility::Hidden);
	}
	GetOwningPlayer()->SetShowMouseCursor(!bShowing);
	AllInventoryItems_Panel->SetIsEnabled(!bShowing);
	BottomInventoryItems_Panel->SetIsEnabled(bShowing);
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

	// All inventory
	InventoryButton = Cast<UZixuanCraftInventoryButton>(AllInventoryItems_Panel->GetChildAt(Index));
	InventoryButton->Update(InSlot);
}

UZixuanCraftInventoryButton* UZixuanCraftMainGameWidget::GetSelectedInventory() const
{
	if (SelectIndex == InvalidIndex)
	{
		return nullptr;
	}

	const TArray<UWidget*>& AllInventory = AllInventoryItems_Panel->GetAllChildren();
	return Cast<UZixuanCraftInventoryButton>(AllInventory[SelectIndex]);
}

void UZixuanCraftMainGameWidget::ResetSelectedInventory()
{
	UZixuanCraftInventoryButton* InventoryButton = nullptr;

	// Bottom Inventory
	if (SelectIndex < BottomInventoryItems_Panel->GetAllChildren().Num())
	{
		InventoryButton = Cast<UZixuanCraftInventoryButton>(BottomInventoryItems_Panel->GetChildAt(SelectIndex));
		InventoryButton->Reset();
	}

	// All inventory
	InventoryButton = Cast<UZixuanCraftInventoryButton>(AllInventoryItems_Panel->GetChildAt(SelectIndex));
	InventoryButton->Reset();
}
