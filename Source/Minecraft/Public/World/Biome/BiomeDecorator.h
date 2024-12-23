// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BiomeDecorator.generated.h"

class AWorldManager;
struct FBlockPos;
class UBiome;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBiomeDecorator : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void Decorate(AWorldManager* InWorldManager, UBiome* Biome, const FBlockPos& InChunkPos);

};
