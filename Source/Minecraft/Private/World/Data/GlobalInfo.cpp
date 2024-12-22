#include "World/Data/GlobalInfo.h"
#include "Utils/ChunkHelper.h"
#include "World/WorldGenerator.h"
#include "Math/ChunkPos.h"
#include "World/Data/ChunkData.h"
#include "Math/BlockPos.h"

void GlobalInfo::Add(const FChunkPos& ChunkPos, TSharedPtr<FChunkData> ChunkData)
{
	CriticalSection.Lock();
	ChunkDataMap.Add(ChunkPos, ChunkData);
	CriticalSection.Unlock();
}

void GlobalInfo::Remove(const FChunkPos& ChunkPos)
{
	CriticalSection.Lock();
	ChunkDataMap.Remove(ChunkPos);
	CriticalSection.Unlock();
}

FBlockState GlobalInfo::GetBlockState(const FIntVector& BlockWorldVoxelLocation)
{
    return GetBlockState(BlockWorldVoxelLocation.X, BlockWorldVoxelLocation.Y, BlockWorldVoxelLocation.Z);
}

FBlockState GlobalInfo::GetBlockState(const FBlockPos& InBlockPos)
{
	return GetBlockState(InBlockPos.X, InBlockPos.Y, InBlockPos.Z);
}

FBlockState GlobalInfo::GetBlockState(int32 X, int32 Y, int32 Z)
{
	// static_cast<float>() 转成float考虑到负数
	//const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldGenerator::CHUNK_SIZE);
	//const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldGenerator::CHUNK_SIZE);

	const FChunkPos ChunkPos = FChunkHelper::ChunkPosFromBlockPos(X, Y);

	FScopeLock ScopeLock(&CriticalSection);
	if (ChunkDataMap.Contains(ChunkPos))
	{
		TSharedPtr<FChunkData> ChunkData = ChunkDataMap[ChunkPos];

		/*FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldGenerator::CHUNK_SIZE;
		const int32 OffsetX = OffsetLocation.X % WorldGenerator::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldGenerator::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;*/

		const int32 OffsetX = X & 15;
		const int32 OffsetY = Y & 15;
		const int32 WorldZ = Z;

		return ChunkData->GetBlockState(OffsetX, OffsetY, WorldZ);
	}

	return FBlockState();
}
