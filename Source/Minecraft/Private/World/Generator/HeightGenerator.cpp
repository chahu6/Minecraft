#include "World/Generator/HeightGenerator.h"
#include "Chunk/Chunk.h"
#include "World/WorldSettings.h"
#include "SimplexNoiseLibrary.h"
#include "Utils/ChunkHelper.h"

void HeightGenerator::GenerateHeight(AChunk* Chunk)
{
	const FVector ChunkLocation = Chunk->GetActorLocation();
	TArray<int32>& HeightMap = Chunk->GetHeightMap();

	for (int32 X = 0; X < WorldSettings::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldSettings::CHUNK_SIZE; ++Y)
		{
			int32 World_X = X * WorldSettings::BlockSize + ChunkLocation.X;
			int32 World_Y = Y * WorldSettings::BlockSize + ChunkLocation.Y;

			//float World_Z = USimplexNoiseLibrary::SimplexNoiseInRange2D(World_X, World_Y, 0, 48, );

			/*float Height = USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f)
				+ 0.5 * USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f * 2)
				+ 0.25 * USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f * 4)
				+ 0.125 * USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f * 8)
				+ 0.0625 * USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f * 16);*/

			float Height = USimplexNoiseLibrary::FBM(World_X, World_Y);

			//Height = FMath::Pow(Height * 1.2f, 0.8f);
			//Height = HeightRemap->GetFloatValue(Height);

			float World_Z = 100.0f + Height * 40.0f;
			//float World_Z = Height;

			HeightMap[ChunkHelper::GetHeightIndex(X, Y)] = FMath::Floor(World_Z);
		}
	}
}
