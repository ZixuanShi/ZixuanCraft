// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "ZixuanCraftWidgetBase.h"
#include "ZixuanCraftMainGameWidget.generated.h"

class AZixuanCraftCharacter;
class UPanelWidget;
class UButton;
class UImage;
class UPanelWidget;
class ALoot;
class UZixuanCraftInventoryButton;

/**
 * User Widget used for main game.
 * - Health
 * - Starveness
 * - Inventory
 * - Mobile input if on Mobile
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftMainGameWidget : public UZixuanCraftWidgetBase
{
	GENERATED_BODY()
	
protected:
	/** All platforms UI */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar_ProgressBar = nullptr;

	/** This panel is at the bottom of the game UI, indicates a portion of inventory that allows the player quick select */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPanelWidget* BottomInventoryItems_Panel = nullptr;

	/** This panel is hide in game, will show up if the player pressed the show key, indicates all the inventory this owning player has */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPanelWidget* AllInventoryItems_Panel = nullptr;

	/** The index to select an item */
	int32 SelectIndex = InvalidIndex;

	/** Mobile dedicated UI */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPanelWidget* Mobile_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Jump_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DestroyAttack_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PlaceUseItem_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ToggleInventory_Mobile_Button = nullptr;


public:
	/**
	 * Update the button in the inventory at Index
	 * @param InSlot		The data to update in the inventory
	 * @param Index			Where the data is in the inventory
	 */
	virtual void IUpdateInventory(const FLootSlot& InSlot, int32 Index) override final;

	virtual void UpdateHealthBarPercent(float Percent) override final { HealthBar_ProgressBar->SetPercent(Percent); }
	virtual void ScrollInventory(bool bIsScrollingDown) override final;
	virtual void IToggleInventory() override final { ToggleInventory(); }
	virtual void IOnJumpButtonPressed() override final { OnJumpButtonPressed(); }
	virtual void IOnJumpButtonReleased() override final { OnJumpButtonReleased(); }
	virtual void IOnDestoryAttackButtonPressed() override final { OnDestoryAttackButtonPressed(); }
	virtual void IOnPlaceUseItemButtonPressed() override final { OnPlaceUseItemButtonPressed(); }
	virtual void ResetInventory(int32 Index) override final;
	virtual int32 GetBottomInventoryNum() const override final { return BottomInventoryItems_Panel->GetAllChildren().Num(); }
	virtual int32 IGetSelectIndex() const override final { return SelectIndex; }
	virtual void SetSelectIndex(int32 Index) override final { SelectIndex = Index; }

public:
	virtual void NativeConstruct() override final;

	/** Turn on and off all inventory */
	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void OnJumpButtonPressed();

	UFUNCTION()
	void OnJumpButtonReleased();

	UFUNCTION()
	void OnDestoryAttackButtonPressed();

	UFUNCTION()
	void OnPlaceUseItemButtonPressed();

	UZixuanCraftInventoryButton* GetSelectedInventory() const;
};
