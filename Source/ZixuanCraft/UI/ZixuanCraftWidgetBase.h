// Fill out your copyright notice in the Description page of Project Settings.
// ZixuanCraftWidgetBase.h
// Author: Zixuan Shi
// Base widget interface for input

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayComponents/LootSlot.h"
#include "Utils/TypeDefs.h"
#include "ZixuanCraftWidgetBase.generated.h"

/**
 * Base class for UI widgets in game
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// Interface
	virtual void IToggleInventory() {}
	virtual void IOnJumpButtonPressed() {}
	virtual void IOnJumpButtonReleased() {}
	virtual void IOnDestoryAttackButtonPressed() {}
	virtual void IOnPlaceUseItemButtonPressed() {}
	virtual int32 IGetSelectIndex() const { return InvalidIndex; }
	virtual void IUpdateInventory(const FLootSlot& InSlot, int32 Index) {};
	virtual void UpdateHealthBarPercent(float Percent) {}
	virtual void ScrollInventory(bool bIsScrollingDown) {};
	virtual int32 GetBottomInventoryNum() const { return InvalidIndex; }
	virtual void ResetItemAt(int32 Index) {};
	virtual void SetSelectIndex(int32 NeWIndex) {};
};
