// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_Log.h"

EBTNodeResult::Type UBTT_Log::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
    GEngine->AddOnScreenDebugMessage(-1, LogDuration, ColorToDisplay, Message);
    return EBTNodeResult::Succeeded;
}
