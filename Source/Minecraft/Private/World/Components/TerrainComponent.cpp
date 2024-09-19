// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Components/TerrainComponent.h"
#include "World/WorldSettings.h"
#include "World/Generator/CaveGenerator.h"
#include "Kismet/KismetMathLibrary.h"
#include "SimplexNoiseLibrary.h"
#include "Math/PoissonDiscSampling.h"
#include "Utils/ChunkHelper.h"
#include "World/WorldManager.h"
#include "World/Block/Blocks.h"

UTerrainComponent::UTerrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTerrainComponent::BeginPlay()
{
	Super::BeginPlay();

	NewNoiseMap.Init(0.f, WorldSettings::CHUNK_AREA);

	// Æ«ÒÆÒòËØ
	OctaveOffsets.SetNum(Octaves);
	for (int32 i = 0; i < Octaves; ++i)
	{
		float OffsetX = FMath::RandRange(-100000.0, 100000.0) + Offset.X;
		float OffsetY = FMath::RandRange(-100000.0, 100000.0) + Offset.Y;
		OctaveOffsets[i] = FVector2D(OffsetX, OffsetY);
	}
}

void UTerrainComponent::LoadTerrainInfo(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos)
{
	GenerateHeight(WorldManager, ChunkVoxelPos);

	LoadTerrainBlockID(WorldManager, ChunkVoxelPos);

	CaveGenerator::GeneratorCave(WorldManager->WorldInfo, ChunkVoxelPos);

	GeneratePlant(WorldManager, ChunkVoxelPos);
}

void UTerrainComponent::LoadTerrainBlockID(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos)
{
	int32 Height = 0;
	int32 MaxHeight = 0;

	FIntPoint ChunkWorldPos = ChunkVoxelPos * WorldSettings::CHUNK_SIZE;

	TArray<int32>& HeightMap = WorldManager->WorldInfo.ChunkDataMap[ChunkVoxelPos]->HeightMap;
	TSharedPtr<FChunkData> ChunkData = WorldManager->WorldInfo.ChunkDataMap[ChunkVoxelPos];

	for (int32 i = 0; i < WorldSettings::CHUNK_AREA; ++i)
	{
		if (HeightMap[i] > MaxHeight)
		{
			MaxHeight = HeightMap[i];
		}
	}

	MaxHeight = MaxHeight > WorldSettings::WATER_LEVEL ? MaxHeight : WorldSettings::WATER_LEVEL;

	for (int32 Y = 0; Y < WorldSettings::CHUNK_SIZE; ++Y)
	{
		for (int32 X = 0; X < WorldSettings::CHUNK_SIZE; ++X)
		{
			Height = HeightMap[ChunkHelper::GetHeightIndex(X, Y)];
			for (int32 Z = 0; Z < MaxHeight + 1; ++Z)
			{
				if (Z > Height)
				{
					if (Z <= WorldSettings::WATER_LEVEL)
					{
						ChunkData->SetBlockState(X, Y, Z, FBlockState(UBlocks::Water, FIntVector(ChunkWorldPos.X + X, ChunkWorldPos.Y + Y, Z)));
						continue;
					}
					break;
				}
				else if (Z == Height)
				{
					ChunkData->SetBlockState(X, Y, Z, FBlockState(UBlocks::Grass, FIntVector(ChunkWorldPos.X + X, ChunkWorldPos.Y + Y, Z)));
				}
				else if (Z > Height - 3)
				{
					ChunkData->SetBlockState(X, Y, Z, FBlockState(UBlocks::Dirt, FIntVector(ChunkWorldPos.X + X, ChunkWorldPos.Y + Y, Z)));
				}
				else
				{
					ChunkData->SetBlockState(X, Y, Z, FBlockState(UBlocks::Stone, FIntVector(ChunkWorldPos.X + X, ChunkWorldPos.Y + Y, Z)));
				}
			}
		}
	}
}

void UTerrainComponent::GenerateHeight(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos)
{
	FIntPoint ChunkWorldPosition = ChunkVoxelPos * WorldSettings::CHUNK_SIZE;

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
	for (int32 X = 0; X < WorldSettings::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldSettings::CHUNK_SIZE; ++Y)
		{
			VoxelWorldX = X + ChunkWorldPosition.X;
			VoxelWorldY = Y + ChunkWorldPosition.Y;

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

			NewNoiseMap[ChunkHelper::GetHeightIndex(X, Y)] = NoiseHeight;
		}
	}

	// ¹æ·¶»¯Îª(0, 1);
	for (int32 i = 0; i < WorldSettings::CHUNK_AREA; ++i)
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

	for (int32 i = 0; i < WorldSettings::CHUNK_AREA; ++i)
	{
		WorldManager->WorldInfo.ChunkDataMap[ChunkVoxelPos]->SetHeight(i, 100 + NewNoiseMap[i] * 28);
	}
}

void UTerrainComponent::GeneratePlant(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos)
{
	TArray<FVector2D> Points;
	PoissonDiscSampling::GeneratePoints(Points, 1.414f, { 16, 16 }, 3);

	TSharedPtr<FChunkData> ChunkData = WorldManager->WorldInfo.ChunkDataMap[ChunkVoxelPos];

	for (const FVector2D& Point : Points)
	{
		int32 X = FMath::TruncToInt32(Point.X);
		int32 Y = FMath::TruncToInt32(Point.Y);
		int32 Z = ChunkData->GetHeight(X, Y);
		ChunkData->SetBlockState(X, Y, Z + 1, FBlockState(UBlocks::Tallgrass, {}));
	}
}

float UTerrainComponent::FBM(float InX, float InY, const TArray<FVector2D>& InOctaveOffsets, ETerrainFBMType InFBMType) const
{
	float NoiseValue = 0.0f;
	float Amplitude = 1.0f;
	float Frequency = 1.0f;
	const float HalfWidth = WorldSettings::CHUNK_SIZE * 0.5f;
	const float HalfHeight = WorldSettings::CHUNK_SIZE * 0.5f;

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
