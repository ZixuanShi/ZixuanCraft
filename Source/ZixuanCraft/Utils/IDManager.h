// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TypeDefs.h"

/**
 * Manager for Id
 */
class ZIXUANCRAFT_API IDManager
{
public:
	static FIndex GetIndex(FId InId);
	static FRefCount GetRefCount(FId InId);
	static FId SetRefCount(FId InId, FRefCount InRefCount);
};
