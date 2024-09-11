#include "World/Generator/CaveGenerator.h"
#include "SimplexNoiseLibrary.h"
#include "World/WorldSettings.h"
#include "Chunk/Chunk.h"
#include "World/Block/Block.h"

void CaveGenerator::GeneratorCave(AChunk* Chunk)
{
	const FVector ChunkLocation = Chunk->GetActorLocation();
	TArray<int32>& HeightMap = Chunk->GetHeightMap();

	for (int32 X = 0; X < WorldSettings::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldSettings::CHUNK_SIZE; ++Y)
		{
			int32 World_X = X * WorldSettings::BlockSize + ChunkLocation.X;
			int32 World_Y = Y * WorldSettings::BlockSize + ChunkLocation.Y;
			for (int32 Z = 10; Z < HeightMap[GetHeightIndex(X, Y)] - 10; ++Z)
			{
				double test = USimplexNoiseLibrary::ImprovedNoise(World_X, World_Y, Z * WorldSettings::BlockSize, 0.001);

				if (test > 0.0)
				{
					Chunk->SetBlock(X, Y, Z, {});
				}
			}
		}
	}
}
