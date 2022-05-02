// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftCraftingResultButton.h"
#include "UI/ZixuanCraftWidgetBase.h"
#include "GameObjects/Crafting/CraftingManager.h"

void UZixuanCraftCraftingResultButton::OnLeftMouseFirstPressedImpl()
{
	Super::OnLeftMouseFirstPressedImpl();
	Data.Reset();

	// For each button in the inventory, substract one from it if it's not empty
	for (UWidget* CraftingWidget : Widget->GetCraftingPanel()->GetAllChildren())
	{
		UZixuanCraftButton* CraftingButton = Cast<UZixuanCraftButton>(CraftingWidget);
		if (CraftingButton->GetData().LootData.Type != EObjectType::Empty)
		{
			--CraftingButton->GetData().Count;
			if (CraftingButton->GetData().Count == 0)
			{
				CraftingButton->Reset();
				CraftingButton->GetData().Reset();
				CraftingManager->UpdateCraftingInput(CraftingButton->GetPanelIndex(), EObjectType::Empty);
			}
			Widget->SetButtonDataAt(CraftingButton->GetData(), CraftingButton->GetWidgetIndex());
		}
	}
}
