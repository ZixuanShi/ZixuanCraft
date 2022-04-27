// Fill out your copyright notice in the Description page of Project Settings.
// ZixuanCraftMainGameWidget.h
// Author: Zixuan Shi
// Main game widget during gameplay

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
class UTextBlock;

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
	//------------------------------------------------------------------------------------------------------------------------------------
	// Gameplay
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UPanelWidget* Gameplay_Panel = nullptr;

	/** All platforms UI */
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UProgressBar* HealthBar_ProgressBar = nullptr;

	/** This panel is at the bottom of the game UI, indicates a portion of inventory that allows the player quick select */
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UPanelWidget* GameplayInventoryItems_Panel = nullptr;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Inventory & Crafting
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UPanelWidget* InventoryCrafting_Panel = nullptr;

	/** Hiden in gameplay, will show up if the player pressed the show key, indicates all the inventory this owning player has */
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UPanelWidget* AllInventoryItems_Panel = nullptr;

	/** Panel for crafting */
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UPanelWidget* Crafting_Panel = nullptr;

	/** The index to select an item */
	int32 SelectedIndex = InvalidIndex;

	//------------------------------------------------------------------------------------------------------------------------------------
	// PC/Console dedicated inventory UI 
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UPanelWidget* SelectedItem_Panel = nullptr;

	/** Image of the item got selected */
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UImage* SelectedItem_Image = nullptr;

	/** Text of the count of the selected item */
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UTextBlock* SelectedItemCount_TextBlock = nullptr;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Mobile dedicated UI
	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UPanelWidget* Mobile_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* Jump_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* DestroyAttack_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* PlaceUseItem_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* ToggleInventory_Mobile_Button = nullptr;

public:
	virtual void NativeConstruct() override final;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override final;
	//------------------------------------------------------------------------------------------------------------------------------------
	// Gameplay
	virtual void UpdateHealthBarPercent(float Percent) override final { HealthBar_ProgressBar->SetPercent(Percent); }
	
	UFUNCTION()
	void OnDestoryAttackButtonPressed();

	UFUNCTION()
	void OnPlaceUseItemButtonPressed();

	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void OnJumpButtonPressed();

	UFUNCTION()
	void OnJumpButtonReleased();

	//------------------------------------------------------------------------------------------------------------------------------------
	// Inventory & Crafting
	/**
	 * Update the button in the inventory at Index
	 * @param InSlot		The data to update in the inventory
	 * @param Index			Where the data is in the inventory
	 */
	virtual void IUpdateInventory(const FLootSlot& InSlot, int32 Index) override final;

	/**
	 * Set the selected item (the one always follows mouse's position)
	 * @param LootSlot		Loot slot data to set
	 */
	virtual void SetSelectedItem(const FLootSlot& LootSlot) override final;

	virtual void ScrollInventory(bool bIsScrollingDown) override final;
	virtual void IToggleInventory() override final { ToggleInventory(); }
	virtual void ResetItemAt(int32 Index) override final;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Mobile
	virtual void IOnJumpButtonPressed() override final { OnJumpButtonPressed(); }
	virtual void IOnJumpButtonReleased() override final { OnJumpButtonReleased(); }
	virtual void IOnDestoryAttackButtonPressed() override final { OnDestoryAttackButtonPressed(); }
	virtual void IOnPlaceUseItemButtonPressed() override final { OnPlaceUseItemButtonPressed(); }

	//------------------------------------------------------------------------------------------------------------------------------------
	// Getter Setters
	virtual int32 GetGameplayInventoryNum() const override final { return GameplayInventoryItems_Panel->GetAllChildren().Num(); }
	virtual int32 IGetSelectIndex() const override final { return SelectedIndex; }
	virtual void SetSelectIndex(int32 Index) override final;
	virtual bool IsDisplayingInventoryPanel() const override final { return InventoryCrafting_Panel->GetIsEnabled(); }
	UZixuanCraftInventoryButton* GetSelectedInventory() const;
};
