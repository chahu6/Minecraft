// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/NBT/KNBTBase.h"
#include "World/Data/BlockState.h"

struct FBlockState;

/**
 * 
 */
class MINECRAFT_API KNBTTagChunkBlockTensor : public KNBTBase
{
public:
	KNBTTagChunkBlockTensor(const FString& InTagName, const TArray<FBlockState>& InValue = TArray<FBlockState>());

	virtual void Read(FArchive& Reader) override;
	virtual void Write(FArchive& Writer) override;
	virtual ENBTTagType GetId() const override;

	TArray<FBlockState> Value;
};
