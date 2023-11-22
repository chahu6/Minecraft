#include "GenLayer.h"
#include "GenLayerIsland.h"

GenLayer::GenLayer(int64 Seed)
{
	BaseSeed = Seed;
	BaseSeed *= BaseSeed * 6364136223846793005L + 1442695040888963407L;
	BaseSeed += Seed;
	BaseSeed *= BaseSeed * 6364136223846793005L + 1442695040888963407L;
	BaseSeed += Seed;
	BaseSeed *= BaseSeed * 6364136223846793005L + 1442695040888963407L;
	BaseSeed += Seed;
}

void GenLayer::InitializeAllBiomeGenerators()
{
	GenLayer* genlayer = new GenLayerIsland(1L);
}

void GenLayer::InitChunkSeed(int64 X, int64 Y)
{
	ChunkSeed = WorldGenSeed;
	ChunkSeed *= ChunkSeed * 6364136223846793005L + 1442695040888963407L;
	ChunkSeed += X;
	ChunkSeed *= ChunkSeed * 6364136223846793005L + 1442695040888963407L;
	ChunkSeed += Y;
	ChunkSeed *= ChunkSeed * 6364136223846793005L + 1442695040888963407L;
	ChunkSeed += X;
	ChunkSeed *= ChunkSeed * 6364136223846793005L + 1442695040888963407L;
	ChunkSeed += Y;
}

int32 GenLayer::NextInt(int32 Seed)
{
	int32 i = (int32)((ChunkSeed >> 24) % (int64)Seed);
	if (i < 0)
	{
		i += Seed;
	}

	ChunkSeed *= ChunkSeed * 6364136223846793005L + 1442695040888963407L;
	ChunkSeed += WorldGenSeed;

	return i;
}
