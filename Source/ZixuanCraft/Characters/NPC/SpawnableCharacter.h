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
	float Health = 50.0f;

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

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** 
	 * @template TDataClass			Blackboard Key's class (i.e. UBlackboardKeyType_Float)
	 * @template TDataType			Blackboard Key's value (i.e. float for UBlackboardKeyType_Float)
	 * @param PropertyName			The name of the property we are trying to get
	 * @return						Blackboard value associated with the given key's name
	 */
	template<class TDataClass, class TDataType>
	TDataType GetProperty(const FName& PropertyName) const;

	/**
	 * Set a value in blackboard
	 * @template TDataClass			Blackboard Key's class (i.e. UBlackboardKeyType_Float)
	 * @template TDataType			Blackboard Key's value (i.e. float for UBlackboardKeyType_Float)
	 * @param PropertyName			The name of the property we are trying to set
	 */
	template<class TDataClass, class TDataType>
	void SetProperty(const FName& PropertyName, TDataType NewValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetState(EAgentState InState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UNPCAnimInstance* GetNPCAnimInstance() const { return Cast<UNPCAnimInstance>(GetMesh()->GetAnimInstance()); }

	float GetHealth() const { return Health; }
	void SetHealth(float NewHealth) { Health = NewHealth; }
};

template<class TDataClass, class TDataType>
inline TDataType ASpawnableCharacter::GetProperty(const FName& PropertyName) const
{
	UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	return Blackboard->GetValue<TDataClass>(PropertyName);
}

template<class TDataClass, class TDataType>
inline void ASpawnableCharacter::SetProperty(const FName& PropertyName, TDataType NewValue)
{
	UBlackboardComponent* Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	Blackboard->SetValue<TDataClass>(PropertyName, NewValue);
}
