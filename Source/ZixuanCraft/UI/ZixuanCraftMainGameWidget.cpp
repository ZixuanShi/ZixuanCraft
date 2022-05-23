// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ZixuanCraftMainGameWidget.h"
#include "UI/ZixuanCraftInventoryButton.h"
#include "UI/ZixuanCraftCraftingButton.h"
#include "UI/ZixuanCraftCraftingResultButton.h"
#include "Characters/Player/ZixuanCraftCharacter.h"
#include "GameObjects/Crafting/CraftingManager.h"
#include "GameplayComponents/InventoryComponent.h"

#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

PRAGMA_DISABLE_OPTIMIZATION

void UZixuanCraftMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitButtons();

	// Initialize player's UI
	AZixuanCraftCharacter* Player = Cast<AZixuanCraftCharacter>(GetOwningPlayerPawn());
	Player->InitWidget(this);

	// Hide the all inventory panel
	ToggleInventory();
}

FReply UZixuanCraftMainGameWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// If right mouse button clicked on a button, and we have something selected
	// we substract 1 count from the selected item to the button clicked if they are the same type
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton &&		// Right mouse button down
		LastHoveredButton &&	// Available button hovered
		IsDisplayingInventoryPanel() &&	// We are displaying inventory/crafting panel
		SelectedSlot.Count > 0 &&	// We have selected a slot
		(SelectedSlot.LootData.Type == LastHoveredButton->GetData().LootData.Type || LastHoveredButton->GetData().LootData.Type == EObjectType::Empty) &&		// They are the same type or clicked is empty
		LastHoveredButton->GetData().Count < MaxSlotCount	// The clicked button is not full
		)	
	{
		LastHoveredButton->OnRightMousePressed();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UZixuanCraftMainGameWidget::ToggleInstructions()
{
	Instructions_Panel->SetIsEnabled(!Instructions_Panel->bIsEnabled);
	if (Instructions_Panel->bIsEnabled)
	{
		Instructions_Panel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Instructions_Panel->SetVisibility(ESlateVisibility::Collapsed);
	}
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

	Cast<UZixuanCraftInventoryButton>(BottomInventory[NewIndex])->OnPressed();
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
		if (AZixuanCraftCharacter* Character = Cast<AZixuanCraftCharacter>(GetOwningPlayerPawn()))
		{
			Character->SetObjectInHand(FLootData());
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

void UZixuanCraftMainGameWidget::SetSelectedItemPanel(const FLootSlot& LootSlot)
{
	SelectedSlot = LootSlot;
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
		SetSelectedItemPanel(FLootSlot{});
	}

	SelectedIndex = Index;
}

UZixuanCraftButton* UZixuanCraftMainGameWidget::GetSelectedInventory() const
{
	if (SelectedIndex == InvalidIndex)
	{
		return nullptr;
	}

	const TArray<UWidget*>& AllInventory = BackpackInventoryItems_Panel->GetAllChildren();
	return Cast<UZixuanCraftInventoryButton>(AllInventory[SelectedIndex]);
}

void UZixuanCraftMainGameWidget::InitButtons()
{
	AZixuanCraftCharacter* Character = Cast<AZixuanCraftCharacter>(GetOwningPlayerPawn());
	int32 WidgetIndex = 0;

	// Gameplay inventory
	for (int32 PanelIndex = 0; PanelIndex < GameplayInventoryItems_Panel->GetAllChildren().Num(); ++PanelIndex)
	{
		Cast<UZixuanCraftInventoryButton>(GameplayInventoryItems_Panel->GetAllChildren()[PanelIndex])->Init(WidgetIndex, PanelIndex, this, Character);
		++WidgetIndex;
	}

	// Backpack
	for (int32 PanelIndex = 0; PanelIndex < BackpackInventoryItems_Panel->GetAllChildren().Num(); ++PanelIndex)
	{
		Cast<UZixuanCraftInventoryButton>(BackpackInventoryItems_Panel->GetAllChildren()[PanelIndex])->Init(WidgetIndex, PanelIndex, this, Character);
		++WidgetIndex;
	}

	// Crafting
	CraftingManager = Cast<ACraftingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACraftingManager::StaticClass()));
	if (CraftingManager)
	{
		for (int32 PanelIndex = 0; PanelIndex < Crafting_Panel->GetAllChildren().Num(); ++PanelIndex)
		{
			Cast<UZixuanCraftCraftingButton>(Crafting_Panel->GetAllChildren()[PanelIndex])->Init(WidgetIndex, PanelIndex, this, Character);
			Cast<UZixuanCraftCraftingButton>(Crafting_Panel->GetAllChildren()[PanelIndex])->SetCraftingManager(CraftingManager);
			++WidgetIndex;
		}
		CraftingResult_Button->Init(WidgetIndex, 0, this, Character);
		CraftingResult_Button->SetCraftingManager(CraftingManager);
		CraftingManager->Init(this, CraftingResult_Button->GetWidgetIndex());
	}

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

UZixuanCraftButton* UZixuanCraftMainGameWidget::GetButtonAt(int32 WidgetIndex) const
{
	if (WidgetIndex == InvalidIndex)
	{
		return nullptr;
	}

	// Gameplay Inventory
	if (WidgetIndex < GetGameplayInventoryNum())
	{
		return Cast<UZixuanCraftButton>(GameplayInventoryItems_Panel->GetChildAt(WidgetIndex));
	}
	// backpack inventory
	else if (WidgetIndex >= GetGameplayInventoryNum() &&
		WidgetIndex < GetTotalInventoryNum())
	{
		return Cast<UZixuanCraftButton>(BackpackInventoryItems_Panel->GetChildAt(ToBackpackIndex(WidgetIndex)));
	}
	// Crafting
	else if (WidgetIndex >= GetTotalInventoryNum() &&
		WidgetIndex < GetTotalInventoryNum() + Crafting_Panel->GetAllChildren().Num())
	{
		return Cast<UZixuanCraftButton>(Crafting_Panel->GetChildAt(ToCraftingIndex(WidgetIndex)));
	}
	else if (WidgetIndex == CraftingResult_Button->GetWidgetIndex())
	{
		return CraftingResult_Button;
	}

	ensureMsgf(false, TEXT("Invalid index for retreiving a button"));
	return nullptr;
}

void UZixuanCraftMainGameWidget::SetButtonDataAt(const FLootSlot& Data, int32 WidgetIndex)
{
	if (WidgetIndex == InvalidIndex)
	{
		return;
	}

	GetButtonAt(WidgetIndex)->SetData(Data);
}

void UZixuanCraftMainGameWidget::ResetItemAt(int32 WidgetIndex)
{
	if (WidgetIndex == InvalidIndex)
	{
		return;
	}

	GetButtonAt(WidgetIndex)->Reset();
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