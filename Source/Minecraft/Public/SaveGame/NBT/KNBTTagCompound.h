// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/NBT/KNBTBase.h"

struct FBlockState;
struct FGameplayTag;

/**
 * 
 */
class MINECRAFT_API KNBTTagCompound : public KNBTBase
{
public:
	KNBTTagCompound(const FString& InTagName);
	~KNBTTagCompound();

	virtual void Read(FArchive& Reader) override;
	virtual void Write(FArchive& Writer) override;
	virtual ENBTTagType GetId() const override;

	KNBTTagCompound& SetInt(const FString& Key, int32 Value);
	KNBTTagCompound& SetChunkBlockDataTensor(const FString& Key, const TArray<FBlockState>& Value);
	KNBTTagCompound& SetChunkBiome(const FString& Key, TArray<FGameplayTag>& Value);

	template<typename T>
	KNBTTagCompound& SetArray(const FString& Key, TArray<T>& Value);

	int32 GetInt(const FString& Key, int32 DefaultValue = 0);
	bool GetChunkBlockDataTensor(const FString& Key, TArray<FBlockState>& OutValue);
	bool GetChunkBiome(const FString& Key, TArray<FGameplayTag>& OutValue);

	template<typename T>
	bool GetArray(const FString& Key, TArray<T>& OutValue);

private:
	TMap<FString, KNBTBase*> Dict;
};
