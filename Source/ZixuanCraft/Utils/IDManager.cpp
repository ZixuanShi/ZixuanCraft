// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/IDManager.h"

FIndex IDManager::GetIndex(FId InId)
{
	return (InId & 0x0000'0000'ffff'ffff);
}

FRefCount IDManager::GetRefCount(FId InId)
{
	return (InId >> 32);
}

FId IDManager::SetRefCount(FId InId, FRefCount InRefCount)
{
	InId &= 0x0000'0000'ffff'ffff;
	InId |= (static_cast<FId>(InRefCount) << 32);
	return InId;
}
