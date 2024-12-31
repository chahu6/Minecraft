// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/ChunkSaveGame.h"
#include "World/Data/ChunkData.h"
#include "World/Block/Block.h"

void UChunkSaveGame::SaveData(const TSharedRef<FChunkData> ChunkData)
{
	HeightMap = ChunkData->HeightMap;
	for (const FGameplayTag Tag : ChunkData->Biomes)
	{
		Biomes.Add(Tag.ToString());
	}
	for (const FBlockState& BlockState : ChunkData->BlockStateMap)
	{
		BlockStateMap.Add(BlockState.GetBlock()->BlockID.ToString());
	}
}
