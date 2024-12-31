// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "ChunkSaveGame.generated.h"

struct FChunkData;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UChunkSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<int16> HeightMap;

	UPROPERTY()
	TArray<FString> Biomes;

	UPROPERTY()
	TArray<FString> BlockStateMap;

	void SaveData(const TSharedRef<FChunkData> ChunkData);
};
