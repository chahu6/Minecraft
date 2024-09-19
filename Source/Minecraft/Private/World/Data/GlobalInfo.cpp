#include "World/Data/GlobalInfo.h"
#include "Utils/ChunkHelper.h"

FBlockState GlobalInfo::GetBlockState(const FIntVector& BlockWorldVoxelLocation)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

	if (ChunkDataMap.Contains(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY)))
	{
		TSharedPtr<FChunkData> ChunkData = ChunkDataMap[FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY)];
		FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldSettings::CHUNK_SIZE;

		const int32 OffsetX = OffsetLocation.X % WorldSettings::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldSettings::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;

		return ChunkData->GetBlockState(OffsetX, OffsetY, WorldZ);
	}
    return FBlockState();
}
