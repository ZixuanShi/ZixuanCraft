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
		Cast<UZixuanCraftInventoryButton>(InventoryButtons)->Init(InventoryButtonIndex);
		++InventoryButtonIndex;
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
	ShowAllInventory(false);

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

void UZixuanCraftMainGameWidget::ShowAllInventory(bool bShowing)
{
	if (bShowing)
	{
		AllInventoryItems_Panel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AllInventoryItems_Panel->SetVisibility(ESlateVisibility::Hidden);
	}
	AllInventoryItems_Panel->SetIsEnabled(bShowing);
}

void UZixuanCraftMainGameWidget::UpdateInventory(const FLootSlot& InSlot, int32 Index)
{
	UZixuanCraftInventoryButton* InventoryButton = nullptr;

	// Bottom
	if (Index < BottomInventoryItems_Panel->GetAllChildren().Num())
	{
		InventoryButton = Cast<UZixuanCraftInventoryButton>(BottomInventoryItems_Panel->GetChildAt(Index));
		InventoryButton->Update(InSlot);
	}

	InventoryButton = Cast<UZixuanCraftInventoryButton>(AllInventoryItems_Panel->GetChildAt(Index));
	InventoryButton->Update(InSlot);
}

