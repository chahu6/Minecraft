#pragma once

class MINECRAFT_API ChunkHelper
{
public:
	static int32 GetBlocksIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ);

	static int32 GetHeightIndex(int32 OffsetX, int32 OffsetY);
};