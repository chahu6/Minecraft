#pragma once

struct FChunkPos;
struct FBlockPos;

class FChunkHelper
{
public:
	static int32 GetBlockIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ);
	static int32 GetBlockIndex(int32 OffsetX, int32 OffsetY);

	static FChunkPos ChunkPosFromBlockPos(const FIntVector& BlockWorldVoxelLocation);
	static FChunkPos ChunkPosFromBlockPos(const FBlockPos& InBlockPos);
	static FChunkPos ChunkPosFromBlockPos(int32 X, int32 Y);

	static FBlockPos BlockPosFromWorldLoc(const FVector& WorldLocation);
	static FChunkPos ChunkPosFromWorldLoc(const FVector& WorldLocation);
	static FChunkPos ChunkPosFromWorldLoc(const FIntVector& WorldLocation);


	static FBlockPos OffsetBlockPosFromBlockPos(const FIntVector& BlockWorldVoxelLocation);
};