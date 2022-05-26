// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Debugging/Debugger.h"
#include "Characters/NPC/AI/AIC_NPC.h"
#include "Characters/NPC/SpawnableCharacter.h"
#include "Characters/NPC/NPCFactory.h"
#include "Characters/Player/BaseCharacter.h"
#include "UI/Debugging/Widgets/DebuggingWidget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UObject/ConstructorHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

PRAGMA_DISABLE_OPTIMIZATION

ADebugger::ADebugger()
{	
	// Load the derived NPC blueprints from memory
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray< FString > ContentPaths;
	ContentPaths.Add(TEXT("/Game/ZixuanCraft/Characters"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	TSet< FName > DerivedNames;
	TArray< FName > BaseNames;
	TSet< FName > Excluded;
	BaseNames.Add(ASpawnableCharacter::StaticClass()->GetFName());
	AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);

	// Get all blueprint classes
	FARFilter Filter;
	Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add(TEXT("/Game/ZixuanCraft/Characters"));
	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	for (const FAssetData& Asset : AssetList)
	{
		// Get the the class this blueprint generates (this is stored as a full path)
		if (Asset.TagsAndValues.Contains(TEXT("GeneratedClass")))
		{
			// Convert path to just the name part
			const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(Asset.GetTagValueRef<FString>(TEXT("GeneratedClass")));
			const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);

			// Check if this class is in the derived set
			if (!DerivedNames.Contains(*ClassName))
			{
				continue;
			}

			// set default pawn class to our Blueprinted character
			ConstructorHelpers::FClassFinder<ASpawnableCharacter> NPCFinder(*ClassObjectPath);
			if (NPCFinder.Class != nullptr)
			{
				NPCBlueprints.Emplace("Spawn " + NPCFinder.Class->GetName(), NPCFinder.Class);
			}
		}
	}
}

void ADebugger::BeginPlay()
{
	Super::BeginPlay();

	// Make sure debugger only 
	check(WITH_EDITOR);

	InitDebuggingWidget();

	// Set TotalAgentCount to existing agents in the map already
	TArray<AActor*> ExistingNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnableCharacter::StaticClass(), ExistingNPCs);
	for (AActor* NPC : ExistingNPCs)
	{
		NPC->OnEndPlay.AddDynamic(this, &ADebugger::OnNPCEndPlay);
		OnTotalAgentsCountChanged(true, Cast<ASpawnableCharacter>(NPC));
	}

	// On NPC Spawn, increment TotalAgentCount, and bind delegate on end play
	OnActorSpawnDebugHandler.BindLambda([this](AActor* SpawnedActor)
		{
			if (SpawnedActor && SpawnedActor->IsA<ASpawnableCharacter>())
			{
				SpawnedActor->OnEndPlay.AddDynamic(this, &ADebugger::OnNPCEndPlay);
				OnTotalAgentsCountChanged(true, Cast<ASpawnableCharacter>(SpawnedActor));
			}
		});
	GetWorld()->AddOnActorSpawnedHandler(OnActorSpawnDebugHandler);
}

void ADebugger::Cheat_SpawnNPC(UClass* NPCClass)
{
	const ABaseCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<ABaseCharacter>();
	const FVector PlayerLocation = Player->GetActorLocation() + FVector(0.0f, 0.0f, 200.0f);
	const FVector SpawnNPCLocation = PlayerLocation + (Player->GetActorForwardVector() * 1000.0f);

	ANPCFactory* NPCFactory = Cast<ANPCFactory>(UGameplayStatics::GetActorOfClass(GetWorld(), ANPCFactory::StaticClass()));
	NPCFactory->SpawnNPC(SpawnNPCLocation, NPCClass);
}

void ADebugger::Cheat_Heal()
{
	ABaseCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<ABaseCharacter>();
	Player->HealToMaxHealth();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Player got healed to max health");
}

void ADebugger::OnNPCEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	OnTotalAgentsCountChanged(false, Cast<ASpawnableCharacter>(Actor));
}

void ADebugger::OnAgentsComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// If selected item got changed to empty, means we killed the NPC that we were debugging. 
	// We need to clear the current UI data
	if (SelectedItem.IsEmpty())
	{
		DebuggingNPC = nullptr;
		DebuggingWidget->UpdateAgentAttackRangeTextBox(TNumericLimits<float>::Max());
		Cast<UTextBlock>(DebuggingWidget->Kill_Button->GetChildAt(0))->SetText(FText::GetEmpty());
	}
	// Normally we update the UI's data by the selected NPC
	else
	{
		DebuggingNPC = AllNPCs[SelectedItem];

		// Attack Range TextBox
		const float AttackRange = DebuggingNPC->GetProperty<UBlackboardKeyType_Float, float>(FName("AttackRange"));
		DebuggingWidget->UpdateAgentAttackRangeTextBox(AttackRange);

		const FString KillButtonString = "Kill: " + DebuggingNPC->GetName();
		Cast<UTextBlock>(DebuggingWidget->Kill_Button->GetChildAt(0))->SetText(FText::FromString(KillButtonString));
	}
}

void ADebugger::OnCheatsComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem.Compare("Heal") == 0)
	{
		Cheat_Heal();
	}
	else
	{
		Cheat_SpawnNPC(NPCBlueprints[SelectedItem]);
	}
}

void ADebugger::OnAttackRangeTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!Text.IsNumeric())
	{
		return;
	}

	const float NewAttackRange = FCString::Atof(*Text.ToString());
	DebuggingNPC->SetProperty<UBlackboardKeyType_Float, float>(FName("AttackRange"), NewAttackRange);

	AAIC_NPC* DebuggingNPCAIController = Cast<AAIC_NPC>(DebuggingNPC->GetController());
	if (NewAttackRange > DebuggingNPCAIController->GetAIPerceptionSightConfig()->SightRadius)
	{
		DebuggingNPCAIController->GetAIPerceptionSightConfig()->SightRadius = NewAttackRange;
		DebuggingNPCAIController->GetAIPerceptionSightConfig()->LoseSightRadius = NewAttackRange;
	}
}

void ADebugger::OnHealthTextBoxTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!Text.IsNumeric())
	{
		return;
	}

	const float NewHealth = FCString::Atof(*Text.ToString());
	DebuggingNPC->SetHealth(NewHealth);
}

void ADebugger::OnKillButtonClicked()
{
	if (DebuggingNPC)
	{
		GetWorld()->DestroyActor(DebuggingNPC);
	}
}

void ADebugger::OnTotalAgentsCountChanged(bool bIsAdding, ASpawnableCharacter* NPC)
{
	if (bIsAdding)
	{
		AllNPCs.Emplace(NPC->GetName(), NPC);
	}
	else
	{
		AllNPCs.Remove(NPC->GetName());
	}

	if (DebuggingWidget)
	{
		DebuggingWidget->SetAgentsCountTextBlock(AllNPCs.Num());
		DebuggingWidget->UpdateAgentsComboBox(bIsAdding, NPC);
	}
}

#if WITH_EDITOR
void ADebugger::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PropertyChangedEvent.Property)
	{
		return;
	}

	if (PropertyChangedEvent.Property->GetFName() == FName("bEnabled"))
	{
		OnEnabledChanged();
	}
}

void ADebugger::OnEnabledChanged()
{
	// Set debugging widget visibility based on bEnabled
	if (DebuggingWidget && bEnabled)
	{
		DebuggingWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DebuggingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

#endif

void ADebugger::InitDebuggingWidget()
{
	// Agents data
	DebuggingWidget->Agents_ComboBox->OnSelectionChanged.AddDynamic(this, &ADebugger::OnAgentsComboBoxSelectionChanged);

	// Cheats
	DebuggingWidget->Cheats_ComboBox->AddOption("Heal");
	for (const auto& Pair : NPCBlueprints)
	{
		DebuggingWidget->Cheats_ComboBox->AddOption(Pair.Key);
	}
	DebuggingWidget->Cheats_ComboBox->OnSelectionChanged.AddDynamic(this, &ADebugger::OnCheatsComboBoxSelectionChanged);

	// Health
	DebuggingWidget->Health_TextBox->OnTextCommitted.AddDynamic(this, &ADebugger::OnHealthTextBoxTextCommitted);

	// Attack Range
	DebuggingWidget->AttackRange_TextBox->OnTextCommitted.AddDynamic(this, &ADebugger::OnAttackRangeTextCommitted);	

	// Kill button
	DebuggingWidget->Kill_Button->OnClicked.AddDynamic(this, &ADebugger::OnKillButtonClicked);
}

PRAGMA_ENABLE_OPTIMIZATION