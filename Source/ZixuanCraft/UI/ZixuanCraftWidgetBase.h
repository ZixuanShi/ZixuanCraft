// Fill out your copyright notice in the Description page of Project Settings.
// ZixuanCraftWidgetBase.h
// Author: Zixuan Shi
// Base widget interface for input

#pragma once
#include "GameplayComponents/LootSlot.h"
#include "Utils/TypeDefs.h"

#include "Blueprint/UserWidget.h"
#include "ZixuanCraftWidgetBase.generated.h"

class UZixuanCraftButton;

/**
 * Base class for UI widgets in game
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	FLootSlot TempHack;

public:
	// Interfaces
	virtual void IToggleInventoryCrafting() {}
	virtual void IOnJumpButtonPressed() {}
	virtual void IOnJumpButtonReleased() {}
	virtual void IOnDestoryAttackButtonPressed() {}
	virtual void IOnPlaceUseItemButtonPressed() {}
	virtual int32 IGetSelectIndex() const { return InvalidIndex; }
	virtual int32 IGetTempHackSelectIndex() const { return InvalidIndex; }
	virtual void SetLastSelectedInventoryIndex(int32 Index) {}
	virtual void UpdateHealthBarPercent(float Percent) {}
	virtual void ScrollInventory(bool bIsScrollingDown) {};
	virtual int32 GetGameplayInventoryNum() const { return InvalidIndex; }
	virtual int32 GetTotalInventoryNum() const { return InvalidIndex; }
	virtual void ResetItemAt(int32 Index) {};
	virtual void SetSelectIndex(int32 NeWIndex) {};
	virtual bool IsDisplayingInventoryPanel() const { return false; }
	virtual void SetSelectedItemPanel(const FLootSlot& InSlot) {};
	virtual void SetButtonDataAt(const FLootSlot& Data, int32 Index) {};
	virtual UZixuanCraftButton* GetButtonAt(int32 WidgetIndex) const { return nullptr; }
	virtual UZixuanCraftButton* GetLastHoveredButton() const { return nullptr; }
	virtual void SetLastHoveredButton(UZixuanCraftButton* Button) {  }
	virtual int32 ToBackpackIndex(int32 WidgetIndex) const { return InvalidIndex; }
	virtual int32 ToCraftingIndex(int32 WidgetIndex) const { return InvalidIndex; }
	virtual const FLootSlot& GetSelectedSlotData() const { return TempHack; }
	virtual bool IsGameplayInventory(int32 WidgetIndex) const { return false; }
	virtual bool IsBackpackInventory(int32 WidgetIndex) const { return false; }
	virtual bool IsCrafting(int32 WidgetIndex) const { return false; }
	virtual FLootSlot& GetSelectedSlotData() { return TempHack; }
};
