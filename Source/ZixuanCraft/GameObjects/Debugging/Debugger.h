// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Debugger.generated.h"

class ASpawnableCharacter;
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
	/** Widget to display debugger data */
	UPROPERTY(BlueprintReadWrite)
	UDebuggingWidget* DebuggingWidget = nullptr;

	/** True to enable debugging widget, false to disable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = true;

	UPROPERTY(BlueprintReadWrite)
	ASpawnableCharacter* DebuggingNPC = nullptr;

	/** Handler when a new Actor spawned in this map */
	FOnActorSpawned::FDelegate OnActorSpawnDebugHandler;

	/** A map storing all active npcs in the map */
	TMap<FString, ASpawnableCharacter*> AllNPCs;

	/** A map storing all available blueprint classes derived from ASpawnableCharacter in memory */
	TMap<FString, UClass*> NPCBlueprints;

public:	
	ADebugger();

	virtual void BeginPlay() override final;

	/** Cheats corresponding functions */
	/** Spawns a NPC near the player */
	UFUNCTION(BlueprintCallable)
	void Cheat_SpawnNPC(UClass* NPCClass);

	UFUNCTION(BlueprintCallable)
	void Cheat_Heal();

	/** Utils */
	UFUNCTION()
	void OnNPCEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	/** Change UI agent display data */
	UFUNCTION()
	void OnAgentsComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnCheatsComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnAttackRangeTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnHealthTextBoxTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnKillButtonClicked();

	void OnTotalAgentsCountChanged(bool bIsAdding, ASpawnableCharacter* NPC);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override final;
	void OnEnabledChanged();
#endif

private:
	void InitDebuggingWidget();
};
