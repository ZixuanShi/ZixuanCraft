// Fill out your copyright notice in the Description page of Project Settings.

#include "GrammarBase.h"

FRuleData::FRuleData()
	: Predecessor{ "0" }
	, Successor{ "S" }
	, Weight{ 0.5f }
{
}

FTerminatingSymbols::FTerminatingSymbols()
	: Symbol{ "S" }
{
}

void AGrammarBase::BeginPlay()
{
	Super::BeginPlay();

	// Make sure two data tables are valid
	check(RulesDataTable);
	check(TSetDataTable);

	// Add Rules to the rule container (P), and non-terminating symbols to N
	for (const FName& Name : RulesDataTable->GetRowNames())
	{
		FRuleData* DataRow = RulesDataTable->FindRow<FRuleData>(Name, "");
		RuleContainer.AddRule(DataRow->Predecessor[0], DataRow->Weight, DataRow->Successor);
		NonTerminatingSymbols.emplace(DataRow->Predecessor[0]);
	}

	// Add Terminating symbols to T
	for (const FName& Name : TSetDataTable->GetRowNames())
	{
		FTerminatingSymbols* TerminatingDataRow = TSetDataTable->FindRow<FTerminatingSymbols>(Name, "");
		TerminatingSymbols.emplace(TerminatingDataRow->Symbol[0]);
	}
}

FString AGrammarBase::GetOutput(const FString& StartSymbol/*="S"*/)
{
	FString Output{ StartSymbol };

	// While output has non terminating symbol at the end of it. 
	while (!Output.IsEmpty() && NonTerminatingSymbols.count(Output[Output.Len() - 1]))
	{
		// Get the last char from Output
		char c = Output[Output.Len() - 1];
		Output.RemoveFromEnd(Output.Right(1));	// Equivalent to std::string::pop_back()

		// Choose a rule from P set using last char.
		Output.Append(RuleContainer.GetRuleStr(c, Rng));
	}

	return Output;
}
