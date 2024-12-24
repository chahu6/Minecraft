#include "Utils/ChunkHelper.h"
#include "World/WorldGenerator.h"
#include "Math/ChunkPos.h"
#include "Math/BlockPos.h"

using namespace WorldGenerator;

int32 FChunkHelper::GetBlockIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ)
{
    //return OffsetX + OffsetY * CHUNK_SIZE + OffsetZ * CHUNK_AREA;
    return OffsetZ << 8 | OffsetY << 4 | OffsetX;
}

int32 FChunkHelper::GetBlockIndex(int32 OffsetX, int32 OffsetY)
{
    //return OffsetX + OffsetY * CHUNK_SIZE;
    return OffsetY << 4 | OffsetX;
}

FChunkPos FChunkHelper::ChunkPosFromBlockPos(const FIntVector& BlockWorldVoxelLocation)
{
    return ChunkPosFromBlockPos(BlockWorldVoxelLocation.X, BlockWorldVoxelLocation.Y);
}

FChunkPos FChunkHelper::ChunkPosFromBlockPos(const FBlockPos& InBlockPos)
{
    return ChunkPosFromBlockPos(InBlockPos.X, InBlockPos.Y);
}

FChunkPos FChunkHelper::ChunkPosFromBlockPos(int32 X, int32 Y)
{
    return FChunkPos(X >> 4, Y >> 4);
}

FBlockPos FChunkHelper::BlockPosFromWorldLoc(const FVector& WorldLocation)
{
    return FBlockPos(FMath::FloorToInt32(WorldLocation.X / BlockSize), FMath::FloorToInt32(WorldLocation.Y / BlockSize), FMath::FloorToInt32(WorldLocation.Z / BlockSize));
}

FChunkPos FChunkHelper::ChunkPosFromWorldLoc(const FVector& WorldLocation)
{
    FChunkPos ChunkPos;
    ChunkPos.X = FMath::FloorToInt32(WorldLocation.X / ChunkSize);
    ChunkPos.Y = FMath::FloorToInt32(WorldLocation.Y / ChunkSize);
    return ChunkPos;
}

FChunkPos FChunkHelper::ChunkPosFromWorldLoc(const FIntVector& WorldLocation)
{
    return ChunkPosFromWorldLoc(FVector(WorldLocation));
}

FBlockPos FChunkHelper::OffsetBlockPosFromBlockPos(const FIntVector& BlockWorldVoxelLocation)
{
    return FBlockPos(BlockWorldVoxelLocation.X & 15, BlockWorldVoxelLocation.Y & 15, BlockWorldVoxelLocation.Z & 15);
}
