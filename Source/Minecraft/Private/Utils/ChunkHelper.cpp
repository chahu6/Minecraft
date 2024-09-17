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
