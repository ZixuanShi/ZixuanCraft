// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

/**
 * Animation instance used for third person shoooters
 */
UCLASS()
class ZIXUANCRAFT_API UNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHitReact = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RandomInt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction = 0.0f;

public:
	void SetHitReact(bool bValue) { bHitReact = bValue; }
	void SetDead(bool bValue) { bDead = bValue; }
	void SetAttacking(bool bValue) { bAttacking = bValue; }
};
