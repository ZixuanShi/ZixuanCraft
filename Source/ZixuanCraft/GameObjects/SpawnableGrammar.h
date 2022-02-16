#pragma once
#include "CoreMinimal.h"
#include "Utils/GrammarBase.h"
#include "Spawnable.h"
#include "SpawnableGrammar.generated.h"

UCLASS()
class ASpawnableGrammar : public AGrammarBase
{
	GENERATED_BODY()

protected:
	// Terminating symbols.
	static constexpr char Loot = 'l';
	static constexpr char Creeper = 'c';
	static constexpr char Zombie = 'z';
	static constexpr char Chicken = 'h';

	// Constants
	static constexpr int LootMax = 3;
	static constexpr int LootMin = 1;

public:
	UFUNCTION(BlueprintCallable)
	FSpawnableParam RunGrammar();

	EObjectType GetSpawnableTypeFromChar(char c) const;
};

