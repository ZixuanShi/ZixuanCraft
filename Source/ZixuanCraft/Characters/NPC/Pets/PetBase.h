// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "PetBase.generated.h"

class UAIPerceptionStimuliSourceComponent;

/**
 * A Pet NPC that fight for the player owner
 */
UCLASS()
class ZIXUANCRAFT_API APetBase : public ASpawnableCharacter
{
	GENERATED_BODY()
	
private:
	/** Creepers flee away from pets if seen and close */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

public:
	APetBase();
};
