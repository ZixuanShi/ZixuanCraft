// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayComponents/LootSlot.h"

#include "Components/Button.h"
#include "ZixuanCraftButton.generated.h"

class UTextBlock;

/**
 * Base class for UI button widget used in the game UI system
 * - If there is no item in the inventory button, show a gray rectangle
 * - If there is a item in the corresponding inventory button, it's background should show the loot slot item's image, and a number indicates the quantity of that loot
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftButton : public UButton
{
	GENERATED_BODY()

protected:
	const FSlateColor PressedColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
	const FSlateColor NormalColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 0.8f));
	const FSlateColor HighlightColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));	// On hover and selected

protected:
	/** Data for this button */
	UPROPERTY(BlueprintReadWrite)
	FLootSlot Data;

	/** The quantity text of this loot */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CountText = nullptr;

	/** This button's index in it's owner panel */
	UPROPERTY(BlueprintReadWrite)
	int32 Index = 0;

public:
	UZixuanCraftButton();

	void Init(int32 NewIndex);
	void Update(const FLootSlot& InSlot);
	void Highlight();
	void Reset();
	virtual void OnPressedImpl();

	UFUNCTION()
	void OnPressed();
};
