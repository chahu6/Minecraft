#pragma once

struct FChunkPos;
struct FBlockPos;

class FChunkHelper
{
public:
	static int32 GetBlocksIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ);

	static int32 GetHeightIndex(int32 OffsetX, int32 OffsetY);

	static FChunkPos ChunkPosFromBlockPos(const FBlockPos& InBlockPos);
	static FChunkPos ChunkPosFromWorldLoc(const FVector& WorldLocation);
	static FChunkPos ChunkPosFromWorldLoc(const FIntVector& WorldLocation);
};