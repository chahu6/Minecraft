// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Biome/BiomeDecorator.h"
#include "World/WorldManager.h"
#include "World/Biome/Biome.h"
#include "World/Gen/WorldGenAbstractTree.h"
#include "Math/BlockPos.h"
#include "World/Gen/WorldGenTallGrass.h"

void UBiomeDecorator::Decorate(AWorldManager* InWorldManager, UBiome* Biome, const FBlockPos& InChunkPos)
{
	// 生成树
	int32 TreeNum = Biome->TreesPerChunk;
	if (FMath::FRand() < Biome->ExtraTreeChance)
	{
		++TreeNum;
	}

	for (int32 i = 0; i < TreeNum; ++i)
	{
		const int32 PosX = FMath::RandHelper(16);
		const int32 PosY = FMath::RandHelper(16);
		const FBlockPos BlockPos = InWorldManager->GetHeight(InChunkPos + FBlockPos(PosX, PosY, 1));

		UWorldGenAbstractTree* WorldGenAbstractTree = Biome->GetRandomTreeFeature();

		if (WorldGenAbstractTree->Generate(InWorldManager, BlockPos))
		{

		}
	}

	// 生成高草
	for (int32 i = 0; i < Biome->GrassPerChunk; ++i)
	{
		const int32 PosX = FMath::RandHelper(16);
		const int32 PosY = FMath::RandHelper(16);
		const FBlockPos GrassPos = InWorldManager->GetHeight(InChunkPos + FBlockPos(PosX, PosY, 1));

		UWorldGenTallGrass* WorldGenTallGrass = Biome->GetRandomWorldGenForGrass();
		WorldGenTallGrass->Generate(InWorldManager, GrassPos);
	}
}
