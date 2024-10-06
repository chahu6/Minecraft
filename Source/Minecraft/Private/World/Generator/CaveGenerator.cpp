#include "World/Generator/CaveGenerator.h"
#include "SimplexNoiseLibrary.h"
#include "World/WorldSettings.h"
#include "World/Data/GlobalInfo.h"
#include "Init/Blocks.h"

void CaveGenerator::GeneratorCave(GlobalInfo& WorldInfo, const FIntPoint& ChunkVoxelPos)
{
	const FIntPoint ChunkLocation = ChunkVoxelPos * WorldSettings::ChunkSize;

	TSharedPtr<FChunkData> ChunkData = WorldInfo.ChunkDataMap[ChunkVoxelPos];

	for (int32 X = 0; X < WorldSettings::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldSettings::CHUNK_SIZE; ++Y)
		{
			int32 World_X = X * WorldSettings::BlockSize + ChunkLocation.X;
			int32 World_Y = Y * WorldSettings::BlockSize + ChunkLocation.Y;
			for (int32 Z = 10; Z < ChunkData->GetHeight(X, Y) - 10; ++Z)
			{
				double test = USimplexNoiseLibrary::ImprovedNoise(World_X, World_Y, Z * WorldSettings::BlockSize, 0.001);

				if (test > 0.0)
				{
					ChunkData->SetBlockState(X, Y, Z, FBlockState(UBlocks::Air));
				}
			}
		}
	}
}
