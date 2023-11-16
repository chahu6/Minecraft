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

	SetBlocksInChunk();
}

// 建筑生成，植物生成，动物生成等地形附加结构。例如矿道，神庙，湖泊等。
void FClassicOverWorldGenerator::Populate(int32 X, int32 Y)
{

}

void FClassicOverWorldGenerator::SetBlocksInChunk()
{
	GenerateBiomeMap();
	GenerateHeightMap();

#if 0

	int32 MaxHeight = 0;
	for (int32 i = 0; i < HeightMap.Num(); ++i)
	{
		if (HeightMap[i] > MaxHeight)
		{
			MaxHeight = HeightMap[i];
		}
	}

	//MaxHeight = MaxHeight > WATER_LEVEL ? MaxHeight : WATER_LEVEL;

	//for (int32 Z = 0; Z < MaxHeight + 1; ++Z)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
		{
			for (int32 X = 0; X < CHUNK_SIZE; ++X)
			{
				int32 Height = HeightMap[GetHeightIndex(X, Y)];

				//float Local_Z = FMath::Min(Height - (ChunkLocation.Z / BlockSize), CHUNK_SIZE);
				//Local_Z = FMath::Floor(Local_Z);

				//if (Z > Height)
				//{
				//	if (Z <= WATER_LEVEL)
				//	{
				//		//CurrentChunk->SetBlock(X, Y, Z, EBlockType::Water);
				//	}
				//	continue;
				//}
				//else if (Z == Height)
				//{
				//	CurrentChunk->SetBlock(X, Y, Z, EBlockType::Grass);
				//}
				//else if (Z > Height - 3)
				//{
				//	CurrentChunk->SetBlock(X, Y, Z, EBlockType::Dirt);
				//}
				//else
				//{
				//	CurrentChunk->SetBlock(X, Y, Z, EBlockType::Stone);
				//}
				//if (Z <= Height)
				//{
					//CurrentChunk->SetBlock(X, Y, Z, 2);
				//}
			}
		}
	}

#endif
}

void FClassicOverWorldGenerator::GenerateBiomeMap()
{

}

void FClassicOverWorldGenerator::GenerateHeightMap()
{
	//for (int32 Relative_X = -7; Relative_X <= 8; ++Relative_X)
	//{
	//	for (int32 Relative_Y = -7; Relative_Y <= 8; ++Relative_Y)
	//	{
	//		float LocationX = GetLocationFromIndex(Relative_X);
	//		float LocationY = GetLocationFromIndex(Relative_Y);

	//		FVector ChunkLocation = CurrentChunk->GetActorLocation();

	//		float Noise_X = LocationX + ChunkLocation.X;
	//		float Noise_Y = LocationY + ChunkLocation.Y;
	//		//float LocationZ = USimplexNoiseLibrary::SimplexNoiseInRange2D(Noise_X, Noise_Y, rangeMin, rangeMax, inFactor);
	//		float LocationZ = USimplexNoiseLibrary::SimplexNoiseInRange2D(Noise_X, Noise_Y, 0, 16, 0.00001f);
	//		int32 Noise_Z = FMath::Floor(LocationZ);

	//		int32 XOffset = Relative_X + 7;
	//		int32 YOffset = Relative_Y + 7;

	//		HeightMap[GetHeightIndex(XOffset, YOffset)] = Noise_Z;
	//	}
	//}

	FVector ChunkLocation = CurrentChunk->GetActorLocation();

	for (int32 X = 0; X < CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
		{
			int32 World_X = X * BlockSize + ChunkLocation.X;
			int32 World_Y = Y * BlockSize + ChunkLocation.Y;

			float World_Z = USimplexNoiseLibrary::SimplexNoiseInRange2D(World_X, World_Y, 0, 48, 0.0001f);
			float Local_Z = FMath::Min(World_Z - (ChunkLocation.Z / BlockSize), CHUNK_SIZE);
			Local_Z = FMath::Floor(Local_Z);

			HeightMap[GetHeightIndex(X, Y)] = FMath::Floor(World_Z);
			for (int32 Z = 0; Z < Local_Z; ++Z)
			{
				CurrentChunk->SetBlock(X, Y, Z, 2);
			}
		}
	}

	// 插值，但是我就直接16 X 16了，以后再优化

}
