// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftCraftingButton.h"

UZixuanCraftCraftingButton::UZixuanCraftCraftingButton()
{
	OnClicked.AddDynamic(this, &UZixuanCraftCraftingButton::OnCraftingPressed);
}

void UZixuanCraftCraftingButton::OnCraftingPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, "Clicked Crafting button");
}
