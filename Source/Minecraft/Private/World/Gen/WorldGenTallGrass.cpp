// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Gen/WorldGenTallGrass.h"
#include "World/WorldManager.h"
#include "Init/Blocks.h"

bool UWorldGenTallGrass::Generate(AWorldManager* InWorldManager, const FBlockPos& Position) const
{
	InWorldManager->SetBlockState(Position, UBlocks::Tallgrass->GetDefaultBlockState());
	return true;
}
