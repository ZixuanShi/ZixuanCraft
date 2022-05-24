// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebuggingWidget.h"
#include "Debugging/Debugger.h"
#include "GameObjects/Terrain/TerrainManager.h"

#include "Kismet/GameplayStatics.h"

void UDebuggingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Debugger = Cast<ADebugger>(UGameplayStatics::GetActorOfClass(GetWorld(), ADebugger::StaticClass()));
	TerrainManager = Cast<ATerrainManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATerrainManager::StaticClass()));
}
