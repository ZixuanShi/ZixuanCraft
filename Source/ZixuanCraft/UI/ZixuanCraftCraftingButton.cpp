// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZixuanCraftCraftingButton.h"

void UZixuanCraftCraftingButton::OnPressedImpl()
{
	Super::OnPressedImpl();

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, "Clicked Crafting button");
}
