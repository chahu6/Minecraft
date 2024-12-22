// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Biome/BiomeDecorator.h"
#include "World/WorldManager.h"
#include "World/Biome/Biome.h"
#include "World/Gen/WorldGenAbstractTree.h"
#include "Math/BlockPos.h"

void UBiomeDecorator::Decorate(AWorldManager* InWorldManager, UBiome* Biome, const FBlockPos& Pos)
{
	int32 TreeNum = Biome->TreesPerChunk;

	if (FMath::FRand() < Biome->ExtraTreeChance)
	{
		++TreeNum;
	}

	for (int32 i = 0; i < TreeNum; ++i)
	{
		int32 PosX = FMath::RandHelper(16);
		int32 PosY = FMath::RandHelper(16);

		UWorldGenAbstractTree* WorldGenAbstractTree = Biome->GetRandomTreeFeature();
		FBlockPos BlockPos = InWorldManager->GetHeight(Pos + FBlockPos(PosX, PosY, 1));

		if (WorldGenAbstractTree->Generate(InWorldManager, BlockPos))
		{

		}
	}
}
