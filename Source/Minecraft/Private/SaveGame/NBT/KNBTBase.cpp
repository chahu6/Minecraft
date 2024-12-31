// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/NBT/KNBTBase.h"
#include "SaveGame/NBT/KNBTTagArray.h"
#include "SaveGame/NBT/KNBTTagCompound.h"
#include "SaveGame/NBT/KNBTTagChunkBlockTensor.h"
#include "SaveGame/NBT/KNBTTagChunkBiome.h"

KNBTBase* KNBTBase::NewTagFromTagId(uint8 Id, const FString& Name)
{
	switch ((ENBTTagType)Id)
	{
		case ENBTTagType::TERMINAL:
			break;
		case ENBTTagType::BYTE:
			return new KNBTTag<uint8>(Name);
		case ENBTTagType::SHORT:
			return new KNBTTag<int16>(Name);
		case ENBTTagType::LONG:
			return new KNBTTag<int64>(Name);
		case ENBTTagType::FLOAT:
			return new KNBTTag<float>(Name);
		case ENBTTagType::DOUBLE:
			return new KNBTTag<double>(Name);
		case ENBTTagType::INT32:
			return new KNBTTag<int32>(Name);
		case ENBTTagType::BYTE_ARRAY:
			return new KNBTTagArray<uint8>(Name);
		case ENBTTagType::INT16_ARRAY:
			return new KNBTTagArray<int16>(Name);
		case ENBTTagType::INT32_ARRAY:
			return new KNBTTagArray<int32>(Name);
		case ENBTTagType::COMPOUND:
			return new KNBTTagCompound(Name);
		case ENBTTagType::CHUNK_BLOCK_TENSOR:
			return new KNBTTagChunkBlockTensor(Name);
		case ENBTTagType::CHUNK_BIOME:
			return new KNBTTagChunkBiome(Name);
		default:
			break;
	}
	check(false);
    return nullptr;
}
