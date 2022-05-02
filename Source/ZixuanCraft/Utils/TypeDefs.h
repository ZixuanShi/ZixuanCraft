#pragma once
// TypeDefs.h
// Author: Zixuan Shi
// Alias for game

#include "Math/NumericLimits.h"

static constexpr int32 InvalidIndex = TNumericLimits<int32>::Max();
static constexpr int32 CraftingPanelSize = 9;

/** Maximum count for a single slot can hold. Used for the loot that can stack in the inventory */
static constexpr int32 MaxSlotCount = 64;