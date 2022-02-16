// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "RNG.h"

using Rule = TPair<float, FString>;

/**
 * Rule container for Grammar
 */
class ZIXUANCRAFT_API FRuleContainer
{
private:
	/** Multimap of Rule instances, keyed by predecessor symbol */
	TMultiMap<char, Rule> Rules;

public:
	/**
	* Add a rule to Rules multi-map
	*
	* @param Variable		Variable to be used as key
	* @param Probability	Probability to trigger the rule
	* @param Rule			The rule to process if triggered	*/
	void AddRule(char Variable, float Probability, const FString& RuleStr);

	/**
	* Returns the rule string by the given Variable as key
	* @param Variable	The key to retreive the rule string
	* @param Rng		The Random number generator to generate random float	*/
	FString GetRuleStr(char Variable, FRNG& Rng);
};
