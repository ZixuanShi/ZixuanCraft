// Fill out your copyright notice in the Description page of Project Settings.
// SpawnableCharacter.h
// Author: Zixuan Shi
// NPC character class used in the game. All NPC should be inherited from this

#pragma once
#include "Utils/Enums.h"
#include "Characters/NPC/Animations/NPCAnimInstance.h"

#include "GameFramework/Character.h"
#include "SpawnableCharacter.generated.h"

class UNPCAnimInstance;
class ATerrainManager;

/**
 * Base character class for all spawnable objects in the game
 */
UCLASS()
class ZIXUANCRAFT_API ASpawnableCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Current & Max health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 30.0f;

	/** How long to destroy the mesh after death. Used for playing the death animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeathLifeSpan = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanPlaySound = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* OnHitSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* OnDeadSound = nullptr;

	FTimerHandle HitSoundTimerHandle;
	ATerrainManager* TerrainManager = nullptr;

public:
	ASpawnableCharacter();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override final;

	UFUNCTION(BlueprintCallable)
	void SetState(EAgentState InState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAgentState GetAgentState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UNPCAnimInstance* GetNPCAnimInstance() const { return Cast<UNPCAnimInstance>(GetMesh()->GetAnimInstance()); }
};
