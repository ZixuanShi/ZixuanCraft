#include "NPCFactory.h"
#include "Characters/NPC/AI/AIC_NPC.h"
#include "Utils/RNG.h"
#include "Debugging/Debugger.h"

#include "Kismet/GameplayStatics.h"

ASpawnableCharacter* ANPCFactory::SpawnNPC(FVector Location)
{
	// Spawn a NPC by weighted random in TerrainManager's SpawnNPCChances map
	const float RandFloat = FRNG::Global().FRand();  // Get a random float within 0-1
	float TotalWeight = 0.0f;
	UClass* NPCToSpawn = nullptr;

	// Weighted random to decide which NPC to spawn
	for (const FNPCSpawnData& NPCSpawnData : SpawnNPCChances)
	{
		const float CurrentWeight = NPCSpawnData.Chance;
		TotalWeight += CurrentWeight;

		if (RandFloat <= TotalWeight)
		{
			NPCToSpawn = NPCSpawnData.Class;
			break;
		}
	}

	// Create Spawned Pawn's AIController and run behavior tree
	ASpawnableCharacter* SpawnedNPC = Cast<ASpawnableCharacter>(GetWorld()->SpawnActor(NPCToSpawn, &Location, &FRotator::ZeroRotator));
	SpawnedNPC->SpawnDefaultController();
	Cast<AAIC_NPC>(SpawnedNPC->Controller)->InitBlackboardData();

	return SpawnedNPC;
}