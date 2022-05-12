// Copyright Epic Games, Inc. All Rights Reserved.

#include "Utils/GPUtils.h"

#include "Engine/Texture2D.h"

namespace GPUtils
{
UTexture2D* TextureFromByteArray(const TArray<uint8>& Bytes, int Width, int Height)
{
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
	FColor* MipData = static_cast<FColor*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	for (int i = 0; i < Bytes.Num() / 4; ++i)
	{
		MipData[i].A = Bytes[4 * i];
		MipData[i].B = Bytes[4 * i + 1];
		MipData[i].G = Bytes[4 * i + 2];
		MipData[i].R = Bytes[4 * i + 3];
	}

	Texture->PlatformData->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();

	return Texture;
}
}
