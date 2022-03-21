// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Characters/ZixuanCraftCharacter.h"
#include "ZixuanCraftWidgetBase.h"
#include "GameplayComponents/LootSlot.h"
#include "ZixuanCraftMainGameWidget.generated.h"

class UProgressBar;
class UPanelWidget;
class UButton;
class UImage;
class UPanelWidget;
class ALoot;

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
	int32 SelectIndex = 0;

	/** Mobile dedicated UI */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPanelWidget* Mobile_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Jump_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DestroyAttack_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PlaceUseItem_Mobile_Button = nullptr;

public:
	virtual void NativeConstruct() override final;

	void ScrollInventoryUp();
	void ScrollInventoryDown();

	/** Turn on and off all inventory */
	void SwitchInventory();

	/**
	 * Update the button in the inventory at Index
	 * @param InSlot		The data to update in the inventory
	 * @param Index			Where the data is in the inventory
	 */
	void UpdateInventory(const FLootSlot& InSlot, int32 Index);
	void UpdateHealthBarPercent(float Percent) { HealthBar_ProgressBar->SetPercent(Percent); }

	void OnJumpButtonPressed() { Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->Jump(); }
	void OnJumpButtonReleased() { Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->StopJumping(); }
	void OnDestoryAttackButtonPressed() { Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->DestroyBlock(); }
	void OnPlaceUseItemButtonPressed() { Cast<AZixuanCraftCharacter>(GetOwningPlayer()->GetPawn())->PlaceBlock(); }

	int32 GetSelectIndex() const { return SelectIndex; }
};
