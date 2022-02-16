#include "SpawnableGrammar.h"

FSpawnableParam ASpawnableGrammar::RunGrammar()
{
	FString Output = GetOutput();

	// Now we have the Output contains terminating symbols only. Time to generate a spawnable using Output
	FSpawnableParam SpawnableParam;
	for (char c : Output)
	{
		// This char must be inside of Terminating symbols at the moment
		check(TerminatingSymbols.count(c));

		// Execute the rules
		switch (c)
		{
		case Loot:
			SpawnableParam.Loot = Rng.RandInRange(LootMin, LootMax);
			break;

		default:
			SpawnableParam.ObjectType = GetSpawnableTypeFromChar(c);
			break;
		}
	}

	FMath::Pow(2, 3);

	// Blueprint child class takes over after this
	return SpawnableParam;
}

EObjectType ASpawnableGrammar::GetSpawnableTypeFromChar(char c) const
{
	switch (c)
	{
	case Creeper:	return EObjectType::Creeper;
	case Zombie:	return EObjectType::Zombie;
	case Chicken:	return EObjectType::Chicken;
	}

	return EObjectType::Creeper;
}
