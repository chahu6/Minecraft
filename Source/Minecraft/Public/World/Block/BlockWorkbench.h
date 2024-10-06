// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Block/Block.h"
#include "BlockWorkbench.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBlockWorkbench : public UBlock
{
	GENERATED_BODY()
public:
	virtual bool OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player) override;
};
