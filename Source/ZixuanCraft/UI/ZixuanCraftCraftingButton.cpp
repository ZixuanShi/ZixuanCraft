// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftCraftingButton.h"
#include "GameObjects/Crafting/CraftingManager.h"

PRAGMA_DISABLE_OPTIMIZATION
void UZixuanCraftCraftingButton::OnRightMousePressed()
{
	Super::OnRightMousePressed();
	NotifyCraftingManager();
}

void UZixuanCraftCraftingButton::OnLeftMouseFirstPressedImpl()
{
	Super::OnLeftMouseFirstPressedImpl();
	Data.Reset();
	NotifyCraftingManager();
}

void UZixuanCraftCraftingButton::OnLeftMouseSecondPressedImpl()
{
	Super::OnLeftMouseSecondPressedImpl();
	NotifyCraftingManager();
}

void UZixuanCraftCraftingButton::NotifyCraftingManager()
{
	CraftingManager->UpdateCraftingInput(PanelIndex, Data.LootData.Type);
}
PRAGMA_ENABLE_OPTIMIZATION
