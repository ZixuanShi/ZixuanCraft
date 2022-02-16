// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZixuanCraftHUD.generated.h"

UCLASS()
class AZixuanCraftHUD : public AHUD
{
	GENERATED_BODY()

public:
	AZixuanCraftHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

