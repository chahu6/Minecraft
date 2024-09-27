#include "Utils/ChunkHelper.h"
#include "World/WorldSettings.h"

int32 ChunkHelper::GetBlocksIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ)
{
    return OffsetX + OffsetY * WorldSettings::CHUNK_SIZE + OffsetZ * WorldSettings::CHUNK_AREA;
}

int32 ChunkHelper::GetHeightIndex(int32 OffsetX, int32 OffsetY)
{
    return OffsetX + OffsetY * WorldSettings::CHUNK_SIZE;
}

FIntPoint ChunkHelper::GetChunkVoxelFromBlockWorldVoxel(const FIntVector& BlockWorldVoxelLocation)
{
    FIntPoint ChunkVoxelLocation;
    ChunkVoxelLocation.X = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
    ChunkVoxelLocation.Y = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

    return ChunkVoxelLocation;
}
