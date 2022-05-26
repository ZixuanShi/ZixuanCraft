#pragma once
#include "Characters/NPC/SpawnableCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCFactory.generated.h"

class ADebugger;

USTRUCT(BlueprintType)
struct FNPCSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Chance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* Class = nullptr;
};

UCLASS()
class ZIXUANCRAFT_API ANPCFactory : public AActor
{
	GENERATED_BODY()

private:
	DECLARE_DELEGATE_OneParam(FOnNPCDestroyed, ASpawnableCharacter*)

private:
	/** Spawning NPC chances for weighted random. The sum of floats must be 1.0f */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FNPCSpawnData> SpawnNPCChances;

public:
	UFUNCTION(BlueprintCallable)
	ASpawnableCharacter* SpawnRandomNPC(FVector Location);

	UFUNCTION(BlueprintCallable)
	ASpawnableCharacter* SpawnNPC(FVector Location, UClass* NPCClass);
};

