// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Gen/WorldGenAbstractTree.h"
#include "WorldGenTrees.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UWorldGenTrees : public UWorldGenAbstractTree
{
	GENERATED_BODY()
	
public:
	virtual bool Generate(AWorldManager* InWorldManager, const FBlockPos& Position) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	int32 MinTreeHeight = 4;

};
