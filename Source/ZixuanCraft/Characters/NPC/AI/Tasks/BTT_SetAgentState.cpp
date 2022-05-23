// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/AI/Tasks/BTT_SetAgentState.h"
#include "Characters/NPC/SpawnableCharacter.h"

#include "AIController.h"

EBTNodeResult::Type UBTT_SetAgentState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASpawnableCharacter* NPC = OwnerComp.GetAIOwner()->GetPawn<ASpawnableCharacter>();
    NPC->SetState(NewState);
    return EBTNodeResult::Succeeded;
}
