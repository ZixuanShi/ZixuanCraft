// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

class UTexture2D;

namespace GPUtils
{
	/** 
	 * Create a UTexture2D from Bytes info and size by width height
	 * @param Bytes		Array of color section to generate texture
	 * @param Width		Width of the new Texture
	 * @param Height	Height of the new Texture
	 */
	UTexture2D* TextureFromByteArray(const TArray<uint8>& Bytes, int Width, int Height);
};
