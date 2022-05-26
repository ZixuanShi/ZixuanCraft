// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Utils/Enums.h"

#include "Blueprint/UserWidget.h" 
#include "DebuggingWidget.generated.h"

class ASpawnableCharacter;
class UComboBoxString;

class UTextBlock;
class UComboBox;
class UEditableTextBox;
class UButton;

/**
 * Widget for debugging purpose, binded with ADebugger. Won't be added to the game if Debugger isn't in the level
 */
UCLASS()
class ZIXUANCRAFT_API UDebuggingWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class ADebugger;

protected:
	/** Used for selecting a cheat to process */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* Cheats_ComboBox = nullptr;

	/** How many agents in the game currently */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AgentsCount_TextBlock = nullptr;

	/** Used for selecting an existing agent in game */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* Agents_ComboBox = nullptr;

	/** The selected agent's current state */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AgentData_TextBlock = nullptr;

	/** The selected agent's current attack range, can be modified */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* AttackRange_TextBox = nullptr;
	
	/** The selected agent's current health */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* Health_TextBox = nullptr;

	/** The selected agent's current target actor */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TargetActor_TextBlock = nullptr;

	/** The selected agent's current target location */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TargetLocation_TextBlock = nullptr;

	/** Press to kill the current debugging NPC */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Kill_Button = nullptr;

	UPROPERTY(BlueprintReadWrite)
	ADebugger* Debugger = nullptr;
 
public:
	void SetAgentsCountTextBlock(int32 InNum);
	void UpdateAgentsComboBox(bool bIsAdding, const ASpawnableCharacter* NPC);
	void UpdateAgentState(EAgentState AgentState);
	void UpdateAgentAttackRangeTextBox(float AttackRange);
	void UpdateTargetActorTextBlock(AActor* TargetActor);
};
