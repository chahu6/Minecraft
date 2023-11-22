#include "GenLayerIsland.h"

GenLayerIsland::GenLayerIsland(int64 Seed)
	:GenLayer(Seed)
{
}

int32* GenLayerIsland::GetInts(int32 AreaX, int32 AreaY, int32 AreaWidth, int32 AreaHeight)
{
	for (int32 i = 0; i < AreaHeight; ++i)
	{
		for (int32 j = 0; j < AreaWidth; ++j)
		{
			InitChunkSeed((int64)(AreaX + j), (int64)(AreaY + i));
			NextInt(10) == 0 ? 1 : 0;
		}
	}
	return nullptr;
}
