#pragma once

class MINECRAFT_API ChunkHelper
{
public:
	static int32 GetBlocksIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ);

	static int32 GetHeightIndex(int32 OffsetX, int32 OffsetY);

	// BlockWorldVoxelLocation (0, 0) - (16, 16)
	static FIntPoint GetChunkVoxelFromBlockWorldVoxel(const FIntVector& BlockWorldVoxelLocation);
};