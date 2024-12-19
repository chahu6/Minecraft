#include "World/Data/ChunkData.h"
#include "Utils/ChunkHelper.h"
#include "World/Block/Block.h"
#include "World/WorldGenerator.h"

FChunkData::FChunkData()
{
	BlockStateMap.Init({}, WorldGenerator::CHUNK_VOLUME);
	HeightMap.Init(0, WorldGenerator::CHUNK_AREA);
}

FBlockState FChunkData::GetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ) const
{
	const int32 Index = FChunkHelper::GetBlocksIndex(OffsetX, OffsetY, OffsetZ);
	if (BlockStateMap.IsValidIndex(Index))
	{
		return BlockStateMap[Index];
	}
	return FBlockState();
}

bool FChunkData::SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockSate)
{
	return SetBlockState(BlockOffsetLocation.X, BlockOffsetLocation.Y, BlockOffsetLocation.Z, BlockSate);
}

bool FChunkData::SetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ, const FBlockState& BlockSate)
{
	const int32 Index = FChunkHelper::GetBlocksIndex(OffsetX, OffsetY, OffsetZ);
	if (BlockStateMap.IsValidIndex(Index))
	{
		BlockStateMap[Index] = BlockSate;
		return true;
	}
	return false;
}

void FChunkData::SetHeight(int32 Index, int32 Height)
{
	if (HeightMap.IsValidIndex(Index))
	{
		HeightMap[Index] = Height;
	}
}

int32 FChunkData::GetHeight(int32 Index) const
{
	if (HeightMap.IsValidIndex(Index))
	{
		return HeightMap[Index];
	}
	return 0;
}

int32 FChunkData::GetHeight(int32 OffsetX, int32 OffsetY) const
{
	return GetHeight(FChunkHelper::GetHeightIndex(OffsetX, OffsetY));
}

void FChunkData::TickUpdate()
{
	for (FBlockState& VoxelState : ActiveVoxels)
	{
		VoxelState.GetBlock()->UpdateTick();
	}
}

