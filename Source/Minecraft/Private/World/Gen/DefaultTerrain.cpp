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

			//��������Ⱥ������
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

	// ʪ��
	float H = Humidity->FBM(4, InX, InY);
	H = (H + 1) / 2; // ȡֵ��Χ[0,1]

	// �¶�
	float T = Temperature->FBM(4, InX, InY);
	T = (T + 1) / 2;

	if (Cont < 0.1f)
	{
		// ������
		if (Ero < 0.f)
		{
			// ����������
			if (T < TEMP_COLD)
			{
				// ���ٵı�ԭ
				OutBiomeTag = GameplayTag.Biomes_FlourishIceField;
			}
			else if (T < TEMP_WARM)
			{
				// �ȴ���ԭ
				OutBiomeTag = GameplayTag.Biomes_Savanna;
			}
			else if (T < TEMP_HOT)
			{
				if (H > HUMIDITY_DRY)
				{
					// ����
					OutBiomeTag = GameplayTag.Biomes_RainForest;
				}
				else
				{
					// �ȴ���ԭ
					OutBiomeTag = GameplayTag.Biomes_Savanna;
				}
			}
		}
		else
		{
			// ɭ��
			OutBiomeTag = GameplayTag.Biomes_Forest;
		}
	}
	else
	{
		// ңԶ����½
		if (Ero < 0.f)
		{
			// ��½ɽ��
			if (T < TEMP_COLD)
			{
				// ƶ�ı�ԭ
				OutBiomeTag = GameplayTag.Biomes_BarrenIceField;
			}
			else
			{
				// ��½ɭ��
				OutBiomeTag = GameplayTag.Biomes_InlandForest;
			}
		}
		else
		{
			if (T > TEMP_WARM)
			{
				// ɳĮ
				OutBiomeTag = GameplayTag.Biomes_Desert;
			}
			else
			{
				// ��½ƽԭ
				OutBiomeTag = GameplayTag.Biomes_InlandPlain;
			}
		}
	}

	return Height;
}

FBlockState UDefaultTerrain::GetBlockState(int32 X, int32 Y, int32 Height, UBiome* Biome, int32 Z) const
{
	// ���ÿշ���
	if (Height <= SEA_LEVEL && Z > SURFACE_HEIGHT && Z == SEA_LEVEL)
	{
		//ˮ
		return UBlocks::Water->GetDefaultBlockState();
	}
	if (Z > Height)
	{
		// ����
		return UBlocks::Air->GetDefaultBlockState();
	}

	// �ھ�Ѩ


	// ���淽��
	if (Z == Height)
	{
		// ����Ⱥ����淽��
		return UBlocks::Grass->GetDefaultBlockState();
	}
	if (Z >= Height - 1 - 7 && Z <= Height - 1)
	{
		// ����Ⱥ��ǳ���
		return UBlocks::Dirt->GetDefaultBlockState();
	}

	// ʯͷ
	return UBlocks::Stone->GetDefaultBlockState();
}
