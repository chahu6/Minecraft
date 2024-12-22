// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Gen/WorldGen.h"
#include "WorldGenAbstractTree.generated.h"

class UBlock;

/**
 * 
 */
UCLASS(abstract)
class MINECRAFT_API UWorldGenAbstractTree : public UWorldGen
{
	GENERATED_BODY()
	
protected:
	bool CanGrowInto(const UBlock* InBlock) const;
};
