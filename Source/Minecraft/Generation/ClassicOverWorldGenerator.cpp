#include "ClassicOverWorldGenerator.h"
#include "SimplexNoiseLibrary.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/MinecraftType/BlockType.h"
#include "Minecraft/World/WorldSettings.h"

FClassicOverWorldGenerator::FClassicOverWorldGenerator()
	:Seed(FMath::Rand())
{
	HeightMap.SetNum(256);
}

FClassicOverWorldGenerator::FClassicOverWorldGenerator(int32 NewSeed)
	:Seed(NewSeed)
{
	HeightMap.SetNum(256);
}

// 基本地形生成，生物群落特有方块覆盖，以及skylight计算。
void FClassicOverWorldGenerator::GenerateChunk(AChunk* Chunk)
{
	CurrentChunk = Chunk;

	ChunkLocation = Chunk->GetActorLocation();

	SetBlocksInChunk();
}

// 建筑生成，植物生成，动物生成等地形附加结构。例如矿道，神庙，湖泊等。
void FClassicOverWorldGenerator::Populate(int32 X, int32 Y)
{

}

void FClassicOverWorldGenerator::SetBlocksInChunk()
{
	//GenerateBiomeMap();
	GenerateHeightMap();

	int32 Height = 0;
	int32 MaxHeight = 0;

	for (int32 i = 0; i < HeightMap.Num(); ++i)
	{
		if (HeightMap[i] > MaxHeight)
		{
			MaxHeight = HeightMap[i];
		}
	}

	MaxHeight = MaxHeight > WATER_LEVEL ? MaxHeight : WATER_LEVEL;

	for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
	{
		for (int32 X = 0; X < CHUNK_SIZE; ++X)
		{
			Height = HeightMap[GetHeightIndex(X, Y)];
			for (int32 Z = 0; Z < MaxHeight; ++Z)
			{
				if (Z > Height)
				{
					if (Z <= WATER_LEVEL)
					{
						CurrentChunk->SetBlock(X, Y, Z, 6);
						continue;
					}
					break;
				}
				else if (Z > Height - 3)
				{
					//CurrentChunk->SetBlock(X, Y, Z, )
				}
				else if (Z == Height)
				{

				}
				else
				{
					CurrentChunk->SetBlock(X, Y, Z, 2);
				}
			}
		}
	}
}

void FClassicOverWorldGenerator::GenerateBiomeMap()
{
	for (int32 X = 0; X < CHUNK_SIZE + 1; ++X)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE + 1; ++Y)
		{

		}
	}
}

void FClassicOverWorldGenerator::GenerateHeightMap()
{
	for (int32 X = 0; X < CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
		{
			int32 World_X = X * BlockSize + ChunkLocation.X;
			int32 World_Y = Y * BlockSize + ChunkLocation.Y;

			//float World_Z = USimplexNoiseLibrary::SimplexNoiseInRange2D(World_X, World_Y, 0, 48, );

			float Height = USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f)
				+ 0.5 * USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f * 2)
				+ 0.25 * USimplexNoiseLibrary::PerlinNoise2D(World_X, World_Y, 0.0001f * 4);

			Height /= 1.75f;

			//Height = FMath::Pow(Height * 1.2f, 0.8f);

			float World_Z = 100.0f + Height * 40.0f;

			HeightMap[GetHeightIndex(X, Y)] = FMath::Floor(World_Z);
		}
	}
}
