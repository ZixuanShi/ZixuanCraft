// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuleContainer.h"
#include "Engine/DataTable.h"
#include "RNG.h"
#include <unordered_set>
#include "GrammarBase.generated.h"

class UDataTable;

/**
* Rules data
*/
USTRUCT(BlueprintType)
struct FRuleData : public FTableRowBase
{
	GENERATED_BODY()

	/** Non-terminating symbol */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString Predecessor;

	/** The rule to apply if predecessor got chosen */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString Successor;

	/** The probability to choose this rule rather than other rules in the same predecessor */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Weight;

	FRuleData();
};

/**
* This table shows all the Terminating symbols
*/
USTRUCT(BlueprintType)
struct FTerminatingSymbols : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString Symbol;

	FTerminatingSymbols();
};

/**
 * Base class for grammar
 */
UCLASS()
class ZIXUANCRAFT_API AGrammarBase : public AActor
{
	GENERATED_BODY()

protected:
	/** We load rules from this table */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* RulesDataTable;

	/** We load terminating symbols from this table */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* TSetDataTable;

	FRNG Rng;

	// Unreal doesn't have built-in unordered set. using std::unordered_set instead here
	// N set. Can't appear on the output
	std::unordered_set<char> NonTerminatingSymbols;

	// T set. Is not necessary in later logic at all. You can delete this if you want
	std::unordered_set<char> TerminatingSymbols;

	// P set. Contains rules.
	FRuleContainer RuleContainer;

public:
	virtual void BeginPlay() override final;

protected:
	/** Returns a output string by grammar */
	FString GetOutput(const FString& StartSymbol = "S");
};
