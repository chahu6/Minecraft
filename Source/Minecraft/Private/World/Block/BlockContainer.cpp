// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Block/BlockContainer.h"
#include "World/WorldManager.h"
#include "TileEntity/TileEntity.h"

ATileEntity* UBlockContainer::CreateNewTileEntity(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation)
{
	if (ATileEntity* TileEntity = WorldManager->GetWorld()->SpawnActorDeferred<ATileEntity>(TileEntityClass, FTransform((FVector(BlockWorldVoxelLocation) + 0.5f) * WorldSettings::BlockSize)))
	{
		TileEntity->SetBlock(this);
		TileEntity->FinishSpawning({}, true);

		return TileEntity;
	}
	return nullptr;
}
