// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Gen/WorldGen.h"
#include "WorldGenTallGrass.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UWorldGenTallGrass : public UWorldGen
{
	GENERATED_BODY()
public:
	virtual bool Generate(AWorldManager* InWorldManager, const FBlockPos& Position) const override;
};
