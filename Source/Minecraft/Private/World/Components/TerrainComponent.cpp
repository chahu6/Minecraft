// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Components/TerrainComponent.h"
#include "World/Generator/HeightGenerator.h"
#include "World/WorldSettings.h"
#include "Chunk/Chunk.h"
#include "World/Block/BlockID.h"
#include "World/Block/Block.h"
#include "World/Generator/CaveGenerator.h"
#include "Kismet/KismetMathLibrary.h"
#include "SimplexNoiseLibrary.h"

UTerrainComponent::UTerrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTerrainComponent::BeginPlay()
{
	Super::BeginPlay();

	NewNoiseMap.Init(0.f, CHUNK_AREA);

	// Æ«ÒÆÒòËØ
	OctaveOffsets.SetNum(Octaves);
	for (int32 i = 0; i < Octaves; ++i)
	{
		float OffsetX = FMath::RandRange(-100000.0, 100000.0) + Offset.X;
		float OffsetY = FMath::RandRange(-100000.0, 100000.0) + Offset.Y;
		OctaveOffsets[i] = FVector2D(OffsetX, OffsetY);
	}
}

void UTerrainComponent::LoadTerrainInfo(AChunk* Chunk)
{
	if (Chunk == nullptr || Chunk->ChunkState != EChunkState::None) return;

	//HeightGenerator::GenerateHeight(Chunk);
	GenerateHeight(Chunk);

	LoadTerrainBlockID(Chunk);

	CaveGenerator::GeneratorCave(Chunk);

	Chunk->RecalculateEmpty();

	Chunk->ChunkState = EChunkState::Loaded;
}

void UTerrainComponent::LoadTerrainBlockID(AChunk* Chunk)
{
	int32 Height = 0;
	int32 MaxHeight = 0;

	TArray<int32>& HeightMap = Chunk->GetHeightMap();

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
			for (int32 Z = 0; Z < MaxHeight + 1; ++Z)
			{
				if (Z > Height)
				{
					if (Z <= WATER_LEVEL)
					{
						Chunk->SetBlock(X, Y, Z, { EBlockID::Water, 0 });
						continue;
					}
					break;
				}
				else if (Z == Height)
				{
					Chunk->SetBlock(X, Y, Z, { EBlockID::Grass, 0 });
				}
				else if (Z > Height - 3)
				{
					Chunk->SetBlock(X, Y, Z, { EBlockID::Dirt, 0 });
				}
				else
				{
					Chunk->SetBlock(X, Y, Z, { EBlockID::Stone, 0 });
				}
			}
		}
	}
}

void UTerrainComponent::GenerateHeight(AChunk* Chunk)
{
	const FVector ChunkLocation = Chunk->GetActorLocation();
	TArray<int32>& HeightMap = Chunk->GetHeightMap();

	float MaxPossibleHeight = 0.f;
	float Amplitude = 1.f;

	for (int32 i = 0; i < Octaves; ++i)
	{
		MaxPossibleHeight += Amplitude;
		Amplitude *= Persistance;
	}
	
	float MaxNoiseHeight = std::numeric_limits<float>::min();
	float MinNoiseHeight = std::numeric_limits<float>::max();

	float NoiseHeight;

	int32 VoxelWorldX;
	int32 VoxelWorldY;
	for (int32 X = 0; X < CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
		{
			VoxelWorldX = X + ChunkLocation.X / BlockSize;
			VoxelWorldY = Y + ChunkLocation.Y / BlockSize;

			int32 World_X = X * BlockSize + ChunkLocation.X;
			int32 World_Y = Y * BlockSize + ChunkLocation.Y;

			if (bDomainWarping)
			{
				const float FBM_x = FBM(VoxelWorldX, VoxelWorldY, OctaveOffsets, WarpingFBM);
				const float FBM_y = FBM(VoxelWorldX + 5.2f, VoxelWorldY + 1.3f, OctaveOffsets, WarpingFBM);

				NoiseHeight = FBM(VoxelWorldX + FBM_x * WarpingStrength, VoxelWorldY + FBM_y * WarpingStrength, OctaveOffsets, BaseFBM);
			}
			else
			{
				NoiseHeight = FBM(VoxelWorldX, VoxelWorldY, OctaveOffsets, BaseFBM);
			}

			if (NoiseHeight > MaxNoiseHeight)
			{
				MaxNoiseHeight = NoiseHeight;
			}
			if (NoiseHeight < MinNoiseHeight)
			{
				MinNoiseHeight = NoiseHeight;
			}

			NewNoiseMap[GetHeightIndex(X, Y)] = NoiseHeight;
		}
	}

	// ¹æ·¶»¯Îª(0, 1);
	for (int32 i = 0; i < CHUNK_AREA; ++i)
	{
		//NewNoiseMap[i] = UKismetMathLibrary::NormalizeToRange(NewNoiseMap[i], MinNoiseHeight, MaxNoiseHeight);
		NewNoiseMap[i] = (NewNoiseMap[i] / MaxPossibleHeight);
	}

	// ÌÝÌï
	//if (bTerrace)
	//{
	//	float DividedHeight, StepX, StepY;
	//	StepX = NoiseInfo.StepSize.X;
	//	StepY = NoiseInfo.StepSize.Y;
	//	for (int32 i = 0; i < Width * Height; ++i)
	//	{
	//		DividedHeight = StepX == 0.0f ? 0.0f : NewNoiseMap[i] / StepX;
	//		switch (NoiseInfo.EdgeType)
	//		{
	//		case ETerraceEdgeType::Normal:
	//			NewNoiseMap[i] = NoiseInfo.Slope == 0.f ? 0.f : FMath::Floor(NewNoiseMap[i] * NoiseInfo.Slope) / NoiseInfo.Slope;
	//			break;
	//		case ETerraceEdgeType::Sharp:
	//			NewNoiseMap[i] = (FMath::Floor(DividedHeight) + FMath::Min(NoiseInfo.Slope * (DividedHeight - FMath::Floor(DividedHeight)), 1.0f)) * StepY;
	//			break;
	//		case ETerraceEdgeType::Smooth:
	//			NoiseInfo.Slope = FMath::Floor(NoiseInfo.Slope * 0.5f) * 2.0f + 1.0f;
	//			NewNoiseMap[i] = (FMath::RoundToFloat(DividedHeight) + 0.5f * FMath::Pow(2.0f * (DividedHeight - FMath::RoundToFloat(DividedHeight)), NoiseInfo.Slope)) * StepY;
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}

	// Ï¿¹È
	/*if (NoiseInfo.bCanyons)
	{
		for (int32 i = 0; i < Width * Height; ++i)
		{
			NewNoiseMap[i] = FMath::Pow(NewNoiseMap[i], NoiseInfo.Power);
		}
	}*/

	for (int32 i = 0; i < CHUNK_AREA; ++i)
	{
		HeightMap[i] = 100 + NewNoiseMap[i] * 28;
	}
}

float UTerrainComponent::FBM(float InX, float InY, const TArray<FVector2D>& InOctaveOffsets, ETerrainFBMType InFBMType) const
{
	float NoiseValue = 0.0f;
	float Amplitude = 1.0f;
	float Frequency = 1.0f;
	const float HalfWidth = CHUNK_SIZE * 0.5f;
	const float HalfHeight = CHUNK_SIZE * 0.5f;

	for (int32 i = 0; i < Octaves; ++i)
	{
		const float SampleX = (InX - HalfWidth + InOctaveOffsets[i].X) / Scale;
		const float SampleY = (InY - HalfHeight + InOctaveOffsets[i].Y) / Scale;
		float PerlinValue = USimplexNoiseLibrary::SimplexNoise2D(SampleX, SampleY, Frequency);
		//float PerlinValue = FMath::PerlinNoise2D(FVector2D(SampleX, SampleY));

		switch (InFBMType)
		{
			case ETerrainFBMType::TurbulenceFBM:
			{
				NoiseValue += FMath::Abs(PerlinValue) * Amplitude;
				break;
			}
			case ETerrainFBMType::RidgeFBM:
			{
				PerlinValue = FMath::Abs(PerlinValue);
				PerlinValue = 1.0f - PerlinValue;
				PerlinValue = PerlinValue * PerlinValue;
				NoiseValue += PerlinValue * Amplitude;
				break;
			}
			default:
			{
				NoiseValue += PerlinValue * Amplitude;
				break;
			}
		}
		Amplitude *= Persistance;
		Frequency *= Lacunarity;
	}

	return NoiseValue;
}

