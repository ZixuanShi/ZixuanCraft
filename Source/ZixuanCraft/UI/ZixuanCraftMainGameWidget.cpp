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
	const TArray<UWidget*>& BottomInventory = GameplayInventoryItems_Panel->GetAllChildren();
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
	ToggleInventory_Mobile_Button->OnPressed.AddDynamic(this, &UZixuanCraftMainGameWidget::ToggleInventory);
#endif
}

void UZixuanCraftMainGameWidget::ScrollInventory(bool bIsScrollingDown)
{
	// If the inventory crafting panel is showing, don't do anything
	const bool bIsShowingInventoryCraftingPanel = IsDisplayingInventoryPanel();
	if (bIsShowingInventoryCraftingPanel)
	{
		return;
	}

	if (SelectIndex == InvalidIndex)
	{
		SelectIndex = 0;
	}

	// Calculate the new index in bottom inventory
	const TArray<UWidget*>& BottomInventory = GameplayInventoryItems_Panel->GetAllChildren();
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

	Cast<UZixuanCraftInventoryButton>(BottomInventory[NewIndex])->OnSelected();
}

void UZixuanCraftMainGameWidget::ToggleInventory()
{
	const bool bIsShowingInventoryCraftingPanel = IsDisplayingInventoryPanel();
	if (bIsShowingInventoryCraftingPanel)
	{
		for (UWidget* InventoryWidget : InventoryCrafting_Panel->GetAllChildren())
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		for (UWidget* GameplayWidget : Gameplay_Panel->GetAllChildren())
		{
			GameplayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		for (UWidget* InventoryWidget : InventoryCrafting_Panel->GetAllChildren())
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
		for (UWidget* GameplayWidget : Gameplay_Panel->GetAllChildren())
		{
			GameplayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	GetOwningPlayer()->SetShowMouseCursor(!bIsShowingInventoryCraftingPanel);
	InventoryCrafting_Panel->SetIsEnabled(!bIsShowingInventoryCraftingPanel);
	Gameplay_Panel->SetIsEnabled(bIsShowingInventoryCraftingPanel);
}

void UZixuanCraftMainGameWidget::IUpdateInventory(const FLootSlot& InSlot, int32 Index)
{
	UZixuanCraftInventoryButton* InventoryButton = nullptr;

	// Bottom Inventory
	if (Index < GameplayInventoryItems_Panel->GetAllChildren().Num())
	{
		InventoryButton = Cast<UZixuanCraftInventoryButton>(GameplayInventoryItems_Panel->GetChildAt(Index));
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

void UZixuanCraftMainGameWidget::ResetItemAt(int32 Index)
{
	if (Index == InvalidIndex)
	{
		return;
	}

	UZixuanCraftInventoryButton* InventoryButton = nullptr;

	// Bottom Inventory
	if (Index < GameplayInventoryItems_Panel->GetAllChildren().Num())
	{
		InventoryButton = Cast<UZixuanCraftInventoryButton>(GameplayInventoryItems_Panel->GetChildAt(Index));
		InventoryButton->Reset();
	}

	// All inventory
	InventoryButton = Cast<UZixuanCraftInventoryButton>(AllInventoryItems_Panel->GetChildAt(Index));
	InventoryButton->Reset();
}

void UZixuanCraftMainGameWidget::OnJumpButtonPressed()
{
	Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->Jump();
}

void UZixuanCraftMainGameWidget::OnJumpButtonReleased()
{
	Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->StopJumping(); 
}

void UZixuanCraftMainGameWidget::OnDestoryAttackButtonPressed()
{ 
	Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->DestroyBlock();
	Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->Attack();
}

void UZixuanCraftMainGameWidget::OnPlaceUseItemButtonPressed()
{ 
	Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->PlaceBlock();
}

