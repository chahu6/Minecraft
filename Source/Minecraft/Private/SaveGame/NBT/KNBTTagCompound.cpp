// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/NBT/KNBTTagCompound.h"
#include "World/Data/BlockState.h"
#include "SaveGame/NBT/KNBTTagChunkBlockTensor.h"
#include "SaveGame/NBT/KNBTTagArray.h"
#include "SaveGame/NBT/KNBTTagChunkBiome.h"

KNBTTagCompound::KNBTTagCompound(const FString& InTagName)
	:KNBTBase(InTagName)
{
}

KNBTTagCompound::~KNBTTagCompound()
{
	for (TPair<FString, KNBTBase*>& Pair : Dict)
	{
		if (Pair.Value)
		{
			delete Pair.Value;
			Pair.Value = nullptr;
		}
	}
}

void KNBTTagCompound::Read(FArchive& Reader)
{
	uint8 Id = 0;
	FString Key;

	Reader << TagName;
	for (;;)
	{
		Reader << Id;
		if (Id == 0) 
			break;
		
		Reader << Key;

		KNBTBase* Tag = NewTagFromTagId(Id, Key);
		Tag->Read(Reader);
		Dict.Add(Key, Tag);
	}
}

void KNBTTagCompound::Write(FArchive& Writer)
{
	uint8 Id = 0;
	uint8 Terminal = 0;

	Writer << TagName;
	for (TPair<FString, KNBTBase*>& Pair : Dict)
	{
		Id = (uint8)Pair.Value->GetId();

		Writer << Id;
		Writer << Pair.Key;
		Pair.Value->Write(Writer);
	}
	Writer.Serialize(&Terminal, 1); // terminal
}

ENBTTagType KNBTTagCompound::GetId() const
{
	return ENBTTagType::COMPOUND;
}

KNBTTagCompound& KNBTTagCompound::SetInt(const FString& Key, int32 Value)
{
	Dict.Add(Key, new KNBTTag<int32>(Key, Value));
	return *this;
}

KNBTTagCompound& KNBTTagCompound::SetChunkBlockDataTensor(const FString& Key, const TArray<FBlockState>& Value)
{
	Dict.Add(Key, new KNBTTagChunkBlockTensor(Key, Value));
	return *this;
}

KNBTTagCompound& KNBTTagCompound::SetChunkBiome(const FString& Key, TArray<FGameplayTag>& Value)
{
	Dict.Add(Key, new KNBTTagChunkBiome(Key, Value));
	return *this;
}

int32 KNBTTagCompound::GetInt(const FString& Key, int32 DefaultValue)
{
	if (Dict.Contains(Key))
	{
		if (KNBTTag<int32>* TagInt = static_cast<KNBTTag<int32>*>(Dict[Key]))
		{
			return TagInt->Value;
		}
	}
	return DefaultValue;
}

bool KNBTTagCompound::GetChunkBlockDataTensor(const FString& Key, TArray<FBlockState>& OutValue)
{
	if (Dict.Contains(Key))
	{
		if (KNBTTagChunkBlockTensor* TagChunkBlockTensor = static_cast<KNBTTagChunkBlockTensor*>(Dict[Key]))
		{
			OutValue = TagChunkBlockTensor->Value;
			return true;
		}
	}
	OutValue = TArray<FBlockState>();
	return false;
}

bool KNBTTagCompound::GetChunkBiome(const FString& Key, TArray<FGameplayTag>& OutValue)
{
	if (Dict.Contains(Key))
	{
		if (KNBTTagChunkBiome* TagChunkBiome = static_cast<KNBTTagChunkBiome*>(Dict[Key]))
		{
			OutValue = TagChunkBiome->Value;
			return true;
		}
	}
	OutValue = TArray<FGameplayTag>();
	return false;
}

template<typename T>
KNBTTagCompound& KNBTTagCompound::SetArray(const FString& Key, TArray<T>& Value)
{
	Dict.Add(Key, new KNBTTagArray<T>(Key, Value));
	return *this;
}

template<typename T>
bool KNBTTagCompound::GetArray(const FString& Key, TArray<T>& OutValue)
{
	if (Dict.Contains(Key))
	{
		if (KNBTTagArray<T>* TagaArray = static_cast<KNBTTagArray<T>*>(Dict[Key]))
		{
			OutValue = TagaArray->Value;
			return true;
		}
	}
	OutValue = TArray<T>();
	return false;
}

template KNBTTagCompound& KNBTTagCompound::SetArray<int16>(const FString& Key, TArray<int16>& Value);
template KNBTTagCompound& KNBTTagCompound::SetArray<int32>(const FString& Key, TArray<int32>& Value);

template bool KNBTTagCompound::GetArray<int16>(const FString& Key, TArray<int16>& Value);
template bool KNBTTagCompound::GetArray<int32>(const FString& Key, TArray<int32>& Value);