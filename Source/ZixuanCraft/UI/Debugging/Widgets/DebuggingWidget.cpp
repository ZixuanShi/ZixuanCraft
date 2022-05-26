// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debugging/Widgets/DebuggingWidget.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "GameObjects/Terrain/TerrainManager.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"

void UDebuggingWidget::SetAgentsCountTextBlock(int32 InNum)
{
	FString NewText = "Agents Count: " + FString::FromInt(InNum);
	AgentsCount_TextBlock->SetText(FText::FromString(NewText));
}

void UDebuggingWidget::UpdateAgentsComboBox(bool bIsAdding, const ASpawnableCharacter* NPC)
{
	if (bIsAdding)
	{
		Agents_ComboBox->AddOption(NPC->GetName());
	}
	else
	{
		Agents_ComboBox->RemoveOption(NPC->GetName());
	}
}

void UDebuggingWidget::UpdateAgentState(EAgentState AgentState)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAgentState"), true);
	const FString AgentStateString = "Agent State: " + EnumPtr->GetNameStringByValue(static_cast<int64>(AgentState));
	AgentData_TextBlock->SetText(FText::FromString(AgentStateString));
}

void UDebuggingWidget::UpdateAgentAttackRangeTextBox(float AttackRange)
{
	if (AttackRange == TNumericLimits<float>::Max())
	{
		AttackRange_TextBox->SetText(FText::FromName("Invalid"));
	}
	else
	{
		AttackRange_TextBox->SetText(FText::AsNumber(AttackRange));
	}
}

void UDebuggingWidget::UpdateTargetActorTextBlock(AActor* TargetActor)
{
	const FString BaseStr("Target Actor: ");

	if (TargetActor)
	{
		TargetActor_TextBlock->SetText(FText::FromString(BaseStr + TargetActor->GetName()));
	}
	else
	{
		TargetActor_TextBlock->SetText(FText::FromString(BaseStr));
	}
}

