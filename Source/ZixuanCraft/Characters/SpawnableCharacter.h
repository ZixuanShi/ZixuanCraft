// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpawnableCharacter.generated.h"

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	Creeper		UMETA(DisplayName = "Creeper"),
	Zombie		UMETA(DisplayName = "Zombie"),
	Chicken		UMETA(DisplayName = "Chicken"),
};

UENUM(BlueprintType)
enum class EAgentState : uint8 
{
	Idle		UMETA(DisplayName = "Idle"),
	Engaged 	UMETA(DisplayName = "Engaged"),
	Dead		UMETA(DisplayName = "Dead"),
};

/**
* Holds all the necessary details of a spawnable*/
USTRUCT(BlueprintType)
struct FSpawnableParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Loot;

	FSpawnableParam();
	FString ToString() const;
};

/**
* Base character class for all spawnable objects in the game
*/
UCLASS()
class ZIXUANCRAFT_API ASpawnableCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnableParam SpawnableParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAgentState AgentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DeathLifeSpan;

public:
	ASpawnableCharacter();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Attack(AActor* Target);
};
