// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/NBT/KNBTTagChunkBiome.h"

KNBTTagChunkBiome::KNBTTagChunkBiome(const FString& InTagName, const TArray<FGameplayTag>& InValue)
	:KNBTBase(InTagName), Value(InValue)
{
}

void KNBTTagChunkBiome::Read(FArchive& Reader)
{
	const FMinecraftGameplayTags& GameplatTags = FMinecraftGameplayTags::Get();
	int32 Num = 0;
	Reader << Num;
	Value.Init(FGameplayTag::EmptyTag, Num);
	biome_t BiomeID = 0;
	for (int32 i = 0; i < Num; ++i)
	{
		Reader << BiomeID;
		Value[i] = GameplatTags.ReverseBiomeLookupTable[BiomeID];
	}
}

void KNBTTagChunkBiome::Write(FArchive& Writer)
{
	const FMinecraftGameplayTags& GameplatTags = FMinecraftGameplayTags::Get();

	int32 Num = Value.Num();
	Writer << Num;
	biome_t BiomeID = 0;
	for (const FGameplayTag& Tag : Value)
	{
		BiomeID = GameplatTags.BiomeLookupTable[Tag];
		Writer << BiomeID;
	}
}

ENBTTagType KNBTTagChunkBiome::GetId() const
{
	return ENBTTagType::CHUNK_BIOME;
}
