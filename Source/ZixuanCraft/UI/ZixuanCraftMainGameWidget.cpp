// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ZixuanCraftMainGameWidget.h"
#include "UI/ZixuanCraftInventoryButton.h"
#include "Characters/ZixuanCraftCharacter.h"

#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

PRAGMA_DISABLE_OPTIMIZATION

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
	// If on mobile, bind events to the button clicks and kill PC/Console dedicated UI
#else
	// Disable PC/Console Dedicated UI
	if (SelectedItem_Panel)
	{
		SelectedItem_Panel->SetIsEnabled(false);
		SelectedItem_Panel->SetVisibility(ESlateVisibility::Hidden);
	}

	// Bind Mobile Button events
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

	if (SelectedIndex == InvalidIndex)
	{
		SelectedIndex = 0;
	}

	// Calculate the new index in bottom inventory
	const TArray<UWidget*>& BottomInventory = GameplayInventoryItems_Panel->GetAllChildren();
	int32 NewIndex = SelectedIndex;
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
		SelectedIndex = InvalidIndex;
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

void UZixuanCraftMainGameWidget::SetSelectedItem(const FLootSlot& LootSlot)
{
	SelectedItem_Image->SetBrushResourceObject(LootSlot.LootData.Icon);

	// If there is something to show, enable the RGBA
	if (LootSlot.Count > 0)
	{
		SelectedItem_Image->SetOpacity(1.0f);
		SelectedItemCount_TextBlock->SetText(FText::AsNumber(LootSlot.Count));
	}
	// Clear RGBA if we don't want to show anything
	else
	{
		SelectedItem_Image->SetOpacity(0.0f);
		SelectedItemCount_TextBlock->SetText(FText::FromString(""));
	}
}

void UZixuanCraftMainGameWidget::SetSelectIndex(int32 Index)
{
	if (Index == InvalidIndex)
	{
		SetSelectedItem(FLootSlot{});
	}

	SelectedIndex = Index;
}

UZixuanCraftInventoryButton* UZixuanCraftMainGameWidget::GetSelectedInventory() const
{
	if (SelectedIndex == InvalidIndex)
	{
		return nullptr;
	}

	const TArray<UWidget*>& AllInventory = AllInventoryItems_Panel->GetAllChildren();
	return Cast<UZixuanCraftInventoryButton>(AllInventory[SelectedIndex]);
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

PRAGMA_ENABLE_OPTIMIZATION