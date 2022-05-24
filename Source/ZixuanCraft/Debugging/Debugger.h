// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Debugger.generated.h"

class UDebuggingWidget;

/**
 * Dedicated for debugging
 * - Debug widget shows agents details and performance
 */
UCLASS()
class ZIXUANCRAFT_API ADebugger : public APawn
{
	GENERATED_BODY()
	
protected:
	/** Set to true to enable debugging features, false to turn if off */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalAgentCount = 0;

	FOnActorSpawned::FDelegate OnActorSpawnDebugHandler;

public:	
	virtual void BeginPlay() override final;

	/** Cheats corresponding functions */
	/** Spawns a NPC near the player */
	UFUNCTION(BlueprintCallable)
	void Cheat_SpawnNPC(UClass* NPCClass);

	UFUNCTION(BlueprintCallable)
	void Cheat_Heal();

	UFUNCTION()
	void OnNPCEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	bool IsEnabled() const { return bEnabled; }
	void UpdateTotalAgentCount(int32 Updater) { TotalAgentCount += Updater; }
};
