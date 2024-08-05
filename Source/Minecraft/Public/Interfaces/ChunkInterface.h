// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ChunkInterface.generated.h"

struct FBlockData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChunkInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINECRAFT_API IChunkInterface
{
	GENERATED_BODY()

public:
	virtual FBlockData GetBlock(int32 X, int32 Y, int32 Z) = 0;

	virtual void SetBlock(int32 X, int32 Y, int32 Z, const FBlockData& BlockData) = 0;
};
