#pragma once

#include "CoreMinimal.h"
#include "World/WorldSettings.h"
#include "World/Data/BlockState.h"

class MINECRAFT_API FChunkData
{
public:
	FIntPoint ChunkPosition;

	TArray<int32> HeightMap;

	TArray<FBlockState> BlockStateMap;

	TArray<FBlockState> ActiveVoxels;

public:
	explicit FChunkData(const FIntPoint& ChunkPos);

	FBlockState GetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ) const;

	void SetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ, const FBlockState& BlockSate);

	void SetHeight(int32 Index, int32 Height);

	int32 GetHeight(int32 Index) const;

	int32 GetHeight(int32 OffsetX, int32 OffsetY) const;

	void TickUpdate();

};