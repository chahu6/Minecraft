#include "World/Data/GlobalInfo.h"
#include "Utils/ChunkHelper.h"
#include "World/WorldGenerator.h"
#include "Math/ChunkPos.h"
#include "World/Data/ChunkData.h"

FBlockState GlobalInfo::GetBlockState(const FIntVector& BlockWorldVoxelLocation)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldGenerator::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldGenerator::CHUNK_SIZE);

	//RWLock.ReadLock();
	CriticalSection.Lock();

	if (ChunkDataMap.Contains(FChunkPos(ChunkVoxelLocationX, ChunkVoxelLocationY)))
	{
		TSharedPtr<FChunkData> ChunkData = ChunkDataMap[FChunkPos(ChunkVoxelLocationX, ChunkVoxelLocationY)];
		//RWLock.ReadUnlock();
		CriticalSection.Unlock();


		FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldGenerator::CHUNK_SIZE;
		const int32 OffsetX = OffsetLocation.X % WorldGenerator::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldGenerator::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;

		return ChunkData->GetBlockState(OffsetX, OffsetY, WorldZ);
	}

	CriticalSection.Unlock();
	//RWLock.ReadUnlock();
    return FBlockState();
}
