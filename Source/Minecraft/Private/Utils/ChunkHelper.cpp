#include "Utils/ChunkHelper.h"
#include "World/WorldGenerator.h"
#include "Math/ChunkPos.h"
#include "Math/BlockPos.h"

using namespace WorldGenerator;

int32 FChunkHelper::GetBlocksIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ)
{
    return OffsetX + OffsetY * CHUNK_SIZE + OffsetZ * CHUNK_AREA;
}

int32 FChunkHelper::GetHeightIndex(int32 OffsetX, int32 OffsetY)
{
    return OffsetX + OffsetY * CHUNK_SIZE;
}

FChunkPos FChunkHelper::ChunkPosFromBlockPos(const FBlockPos& InBlockPos)
{
    FChunkPos ChunkPos;
    ChunkPos.X = FMath::FloorToInt32(static_cast<float>(InBlockPos.X) / CHUNK_SIZE);
    ChunkPos.Y = FMath::FloorToInt32(static_cast<float>(InBlockPos.Y) / CHUNK_SIZE);
    return ChunkPos;
}

FChunkPos FChunkHelper::ChunkPosFromWorldLoc(const FVector& WorldLocation)
{
    return ChunkPosFromWorldLoc(FIntVector(WorldLocation));
}

FChunkPos FChunkHelper::ChunkPosFromWorldLoc(const FIntVector& WorldLocation)
{
    FChunkPos ChunkPos;
    ChunkPos.X = FMath::FloorToInt32(static_cast<float>(WorldLocation.X) / ChunkSize);
    ChunkPos.Y = FMath::FloorToInt32(static_cast<float>(WorldLocation.Y) / ChunkSize);
    return ChunkPos;
}
