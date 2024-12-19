// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Gen/DefaultTerrain.h"
#include "World/WorldGenerator.h"
#include "World/Gen/SimplexNoiseData.h"
#include "GameplayTagContainer.h"
#include "MinecraftGameplayTags.h"
#include "World/Biome/Biome.h"
#include "World/Data/BlockState.h"
#include "Init/Blocks.h"
#include "Math/ChunkPos.h"
#include "Math/BlockPos.h"
#include "World/WorldManager.h"
#include "World/Data/ChunkData.h"

using namespace WorldGenerator;

void UDefaultTerrain::BeginPlay()
{
}

void UDefaultTerrain::Generate_Implementation(AWorldManager* InWorldManager, const FChunkPos& InChunkPos)
{
	FBlockPos BlockPos = InChunkPos.ToBlockPos();
	TSharedPtr<FChunkData> ChunkData = InWorldManager->WorldInfo.ChunkDataMap[InChunkPos];

	int32 BlockPosX = 0;
	int32 BlockPosY = 0;
	const float HalfWidth = WorldGenerator::CHUNK_SIZE * 0.5f;
	const float HalfHeight = WorldGenerator::CHUNK_SIZE * 0.5f;

	for (int32 X = 0; X < WorldGenerator::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldGenerator::CHUNK_SIZE; ++Y)
		{
			BlockPosX = X + BlockPos.X;
			BlockPosY = Y + BlockPos.Y;

			const float SampleX = (BlockPosX - HalfWidth) / Scale;
			const float SampleY = (BlockPosY - HalfHeight) / Scale;

			FGameplayTag BiomeTag;
			int32 RealHeight = GetRealHeightAndBiomes(SampleX, SampleY, BiomeTag);
			UBiome* Biome = UBiome::GetBiome(BiomeTag);
			//check(Biome);

			for (int32 Z = 0; Z < CHUNK_HEIGHT; ++Z)
			{
				//FVector PointPos = InChunk->GetActorLocation();

				FBlockState BlockState = GetBlockState(X, Y, RealHeight, Biome, Z);
				ChunkData->SetBlockState(FIntVector(X, Y, Z), BlockState);
			}

			if (RealHeight <= SEA_LEVEL)
			{
				continue;
			}

			//生成生物群落条件
			//int32 LeafWidth = 
		}
	}
}

int32 UDefaultTerrain::GetRealHeightAndBiomes(float InX, float InY, FGameplayTag& OutBiomeTag)
{
	const FMinecraftGameplayTags& GameplayTag = FMinecraftGameplayTags::Get();

	const float Cont = Continent->FBM(4, InX, InY);
	const float Ero = Erosion->FBM(5, InX, InY);
	const float Pea = PeaksValleys->FBM(4, InX, InY);

	float Height = 0.f;
	Height = ContinentalnessCurve->GetFloatValue(Cont);
	Height += ErosionCurve->GetFloatValue(Ero);
	Height += PeakValleysCurve->GetFloatValue(Pea);

	if (Height >= WORLD_DEPTH * 0.95f) Height = WORLD_DEPTH * 0.95f;
	if (Height < 0) Height = 0;

	// 湿度
	float H = Humidity->FBM(4, InX, InY);
	H = (H + 1) / 2; // 取值范围[0,1]

	// 温度
	float T = Temperature->FBM(4, InX, InY);
	T = (T + 1) / 2;

	if (Cont < 0.1f)
	{
		// 海岸线
		if (Ero < 0.f)
		{
			// 海岸线丘陵
			if (T < TEMP_COLD)
			{
				// 繁荣的冰原
				OutBiomeTag = GameplayTag.Biomes_FlourishIceField;
			}
			else if (T < TEMP_WARM)
			{
				// 热带草原
				OutBiomeTag = GameplayTag.Biomes_Savanna;
			}
			else if (T < TEMP_HOT)
			{
				if (H > HUMIDITY_DRY)
				{
					// 雨林
					OutBiomeTag = GameplayTag.Biomes_RainForest;
				}
				else
				{
					// 热带草原
					OutBiomeTag = GameplayTag.Biomes_Savanna;
				}
			}
		}
		else
		{
			// 森林
			OutBiomeTag = GameplayTag.Biomes_Forest;
		}
	}
	else
	{
		// 遥远的内陆
		if (Ero < 0.f)
		{
			// 内陆山脉
			if (T < TEMP_COLD)
			{
				// 贫瘠的冰原
				OutBiomeTag = GameplayTag.Biomes_BarrenIceField;
			}
			else
			{
				// 内陆森林
				OutBiomeTag = GameplayTag.Biomes_InlandForest;
			}
		}
		else
		{
			if (T > TEMP_WARM)
			{
				// 沙漠
				OutBiomeTag = GameplayTag.Biomes_Desert;
			}
			else
			{
				// 内陆平原
				OutBiomeTag = GameplayTag.Biomes_InlandPlain;
			}
		}
	}

	return Height;
}

FBlockState UDefaultTerrain::GetBlockState(int32 X, int32 Y, int32 Height, UBiome* Biome, int32 Z) const
{
	// 放置空方块
	if (Height <= SEA_LEVEL && Z > SURFACE_HEIGHT && Z == SEA_LEVEL)
	{
		//水
		return UBlocks::Water->GetDefaultBlockState();
	}
	if (Z > Height)
	{
		// 空气
		return UBlocks::Air->GetDefaultBlockState();
	}

	// 挖掘洞穴


	// 表面方块
	if (Z == Height)
	{
		// 生物群落表面方块
		return UBlocks::Grass->GetDefaultBlockState();
	}
	if (Z >= Height - 1 - 7 && Z <= Height - 1)
	{
		// 生物群落浅表块
		return UBlocks::Dirt->GetDefaultBlockState();
	}

	// 石头
	return UBlocks::Stone->GetDefaultBlockState();
}
