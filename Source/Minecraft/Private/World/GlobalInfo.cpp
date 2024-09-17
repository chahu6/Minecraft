#include "World/GlobalInfo.h"
#include "Utils/ChunkHelper.h"

int32 GlobalInfo::GetBlock(const FIntVector& BlockWorldVoxelLocation)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

	if (ChunkData* Data = ChunksMap.Find(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY)))
	{
		FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldSettings::CHUNK_SIZE;

		const int32 OffsetX = OffsetLocation.X % WorldSettings::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldSettings::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;

		if (WorldZ < 0 || WorldZ >= WorldSettings::CHUNK_HEIGHT)
		{
			return 0;
		}

		auto t = ChunkHelper::GetBlocksIndex(OffsetX, OffsetY, WorldZ);
		return Data->BlocksMap[ChunkHelper::GetBlocksIndex(OffsetX, OffsetY, WorldZ)];
	}
    return 0;
}
