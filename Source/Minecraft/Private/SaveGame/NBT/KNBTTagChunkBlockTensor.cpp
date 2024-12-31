// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/NBT/KNBTTagChunkBlockTensor.h"
#include "World/Block/Block.h"
#include "MinecraftGameplayTags.h"

KNBTTagChunkBlockTensor::KNBTTagChunkBlockTensor(const FString& InTagName, const TArray<FBlockState>& InValue)
	:KNBTBase(InTagName), Value(InValue)
{
}

void KNBTTagChunkBlockTensor::Read(FArchive& Reader)
{
	const FMinecraftGameplayTags& GameplatTags = FMinecraftGameplayTags::Get();
	int32 Num = 0;
	Reader << Num;

	Value.Init({}, Num);
	item_t BlockID = 0;
	for (int32 i = 0; i < Num; ++i)
	{
		Reader << BlockID;

		Value[i].SetBlock(UBlock::GetBlockByID(GameplatTags.ReverseItemLookupTable[BlockID]));
		Reader << Value[i].State;
	}
}

void KNBTTagChunkBlockTensor::Write(FArchive& Writer)
{
	const FMinecraftGameplayTags& GameplatTags = FMinecraftGameplayTags::Get();
	item_t BlockID = 0;
	int32 Num = Value.Num();
	Writer << Num;
	for (FBlockState& BlockState : Value)
	{
		BlockID = GameplatTags.ItemLookupTable[BlockState.GetBlock()->BlockID];
		Writer << BlockID;
		Writer << BlockState.State;
	}
}

ENBTTagType KNBTTagChunkBlockTensor::GetId() const
{
	return ENBTTagType::CHUNK_BLOCK_TENSOR;
}
