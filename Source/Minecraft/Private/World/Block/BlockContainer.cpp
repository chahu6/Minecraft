// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Block/BlockContainer.h"
#include "World/WorldManager.h"
#include "TileEntity/TileEntity.h"

ATileEntity* UBlockContainer::CreateNewTileEntity(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation)
{
	return WorldManager->GetWorld()->SpawnActor<ATileEntity>(TileEntityClass, (FVector(BlockWorldVoxelLocation) + 0.5f) * WorldSettings::BlockSize, FRotator::ZeroRotator);
}
