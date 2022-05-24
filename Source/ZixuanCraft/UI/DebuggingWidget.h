// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebuggingWidget.generated.h"

class ADebugger;
class ATerrainManager;

class UTextBlock;

/**
 * Widget for debugging purpose, binded with ADebugger
 */
UCLASS()
class ZIXUANCRAFT_API UDebuggingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	ADebugger* Debugger = nullptr;

	UPROPERTY(BlueprintReadWrite)
	ATerrainManager* TerrainManager = nullptr;
	
	/** How many agents in the game currently */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AgentCount = nullptr;

public:
	virtual void NativeConstruct() override final;
};
