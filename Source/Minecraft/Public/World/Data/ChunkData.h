#pragma once

#include "CoreMinimal.h"
#include "World/WorldSettings.h"

class MINECRAFT_API ChunkData
{
public:
	ChunkData(const FIntPoint& ChunkPos)
		:ChunkPosition(ChunkPos),
		HeightMap{0},
		BlocksMap{0}
	{}

	FIntPoint ChunkPosition;

	int32 HeightMap[WorldSettings::CHUNK_AREA];

	int32 BlocksMap[WorldSettings::CHUNK_VOLUME];
};