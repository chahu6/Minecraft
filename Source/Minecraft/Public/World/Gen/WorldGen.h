// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WorldGen.generated.h"

class AWorldManager;
struct FBlockPos;

/**
 * 
 */
UCLASS(abstract)
class MINECRAFT_API UWorldGen : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual bool Generate(AWorldManager* InWorldManager, const FBlockPos& Position) const;
};
