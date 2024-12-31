// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/NBT/KNBTBase.h"
#include "MinecraftGameplayTags.h"

struct FGameplayTag;

/**
 * 
 */
class MINECRAFT_API KNBTTagChunkBiome : public KNBTBase
{
public:
	KNBTTagChunkBiome(const FString& InTagName, const TArray<FGameplayTag>& InValue = TArray<FGameplayTag>());

	virtual void Read(FArchive& Reader) override;
	virtual void Write(FArchive& Writer) override;
	virtual ENBTTagType GetId() const override;

public:
	TArray<FGameplayTag> Value;
};
