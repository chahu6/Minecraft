// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "World/Block/Block.h"
#include "ChunkSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UChunkSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<EBlockID> Blocks;
};
