// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZixuanCraftButton.h"
#include "ZixuanCraftCraftingButton.generated.h"

/**
 * Button used for crafting
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftCraftingButton : public UZixuanCraftButton
{
	GENERATED_BODY()

public:
	virtual void OnPressedImpl() override final;
};
