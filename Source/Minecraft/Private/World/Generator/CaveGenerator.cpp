#include "World/Generator/CaveGenerator.h"
#include "SimplexNoiseLibrary.h"
#include "World/WorldSettings.h"
#include "Chunk/Chunk.h"
#include "World/Block/Block.h"

void CaveGenerator::GeneratorCave(AChunk* Chunk)
{
	const FVector ChunkLocation = Chunk->GetActorLocation();
	TArray<uint8>& HeightMap = Chunk->GetHeightMap();

	for (int32 X = 0; X < CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
		{
			int32 World_X = X * BlockSize + ChunkLocation.X;
			int32 World_Y = Y * BlockSize + ChunkLocation.Y;
			for (int32 Z = 10; Z < HeightMap[GetHeightIndex(X, Y)] - 10; ++Z)
			{
				double test = USimplexNoiseLibrary::ImprovedNoise(World_X, World_Y, Z);
				test = (test + 1) / 2;

				if (test > 0.4f)
				{
					Chunk->SetBlock(X, Y, Z, {});
				}
			}
		}
	}
}
