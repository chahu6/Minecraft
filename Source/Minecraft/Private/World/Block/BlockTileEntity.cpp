// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Block/BlockTileEntity.h"
#include "World/WorldManager.h"
#include "TileEntity/TileEntity.h"
#include "World/WorldGenerator.h"

ATileEntity* UBlockTileEntity::CreateNewTileEntity_Implementation(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation)
{
	if (ATileEntity* TileEntity = WorldManager->GetWorld()->SpawnActorDeferred<ATileEntity>(TileEntityClass, FTransform((FVector(BlockWorldVoxelLocation) + 0.5f) * WorldGenerator::BlockSize)))
	{
		TileEntity->SetBlock(this);
		TileEntity->FinishSpawning({}, true);

		return TileEntity;
	}
	return nullptr;
}

EBlockRenderType UBlockTileEntity::GetRenderType() const
{
	return EBlockRenderType::Skeletal;
}
