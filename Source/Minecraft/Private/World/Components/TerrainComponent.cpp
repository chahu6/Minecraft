// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Components/TerrainComponent.h"
#include "World/Generator/HeightGenerator.h"
#include "World/WorldSettings.h"
#include "Chunk/Chunk.h"
#include "World/Block/BlockID.h"
#include "World/Block/Block.h"

UTerrainComponent::UTerrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTerrainComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTerrainComponent::LoadTerrainInfo(AChunk* Chunk)
{
	if (Chunk == nullptr) return;

	HeightGenerator::GenerateHeight(Chunk);

	LoadTerrainBlockID(Chunk);

	Chunk->UpdateBlock();
}

void UTerrainComponent::LoadTerrainBlockID(AChunk* Chunk)
{
	int32 Height = 0;
	int32 MaxHeight = 0;

	TArray<uint8>& HeightMap = Chunk->GetHeightMap();

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

