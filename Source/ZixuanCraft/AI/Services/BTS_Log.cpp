// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTS_Log.h"

void UBTS_Log::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
    GEngine->AddOnScreenDebugMessage(-1, LogDuration, ColorToDisplay, Message);
}
