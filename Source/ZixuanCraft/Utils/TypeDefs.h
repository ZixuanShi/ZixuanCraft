#pragma once
#include "Math/NumericLimits.h"
#include <unordered_set>

// 64 bits signed integer contains both RefCount and Index. 
// Using int64 instead of uint64 just for blueprint read write
using FId = int64;			

// Second half of FId
using FIndex = int32;		

// First half of FId
using FRefCount = int32;	

// arrays of node index.
using NodeSequence = TArray<FIndex>;

// An array of arrays of node in.
using NodeSequenceList = TArray<NodeSequence>;

// Discovered Set
using DiscoveredSet = std::unordered_set<FIndex>;

static constexpr FId InvalidId = TNumericLimits<int64>::Max();
static constexpr int32 InvalidIndex = TNumericLimits<int32>::Max();