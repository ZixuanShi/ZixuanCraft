// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleContainer.h"

void FRuleContainer::AddRule(char Variable, float Probability, const FString& RuleStr)
{
    check(Probability <= 1.0f);
    Rules.Emplace(Variable, Rule{ Probability, RuleStr });
}

FString FRuleContainer::GetRuleStr(char Variable, FRNG& Rng)
{
    // Get all rules at key
    TArray<Rule> OutArray;
    Rules.MultiFind(Variable, OutArray);

    float TotalWeight = 0.0f;       // Will be used for picking a rule
    float RandFloat = Rng.FRand();  // Get a random float within 0-1

    // Loop through OutArray
    for (const Rule& CurrentRule : OutArray)
    {
        float RuleWeight = CurrentRule.Key;
        TotalWeight += RuleWeight;

        // if TotalWeight is greater than RandFloat
        if (RandFloat < TotalWeight)
        {
            // Return current rule
            return CurrentRule.Value;
        }
    }

    // We should never got here
    check(false);
    return FString();
}