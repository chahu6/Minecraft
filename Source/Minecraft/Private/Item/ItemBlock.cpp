// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBlock.h"
#include "World/WorldManager.h"
#include "Player/EntityPlayer.h"
#include "World/Block/Block.h"

UBlock* UItemBlock::GetBlock() const
{
    return Block;
}

bool UItemBlock::OnItemUse(AEntityPlayer* Player, AWorldManager* WorldManager, const FIntVector& BlockVoxelLocation, const FVector& HitNormal)
{
    if (WorldManager == nullptr) return false;

    FIntVector PlaceBlockVoxelLocation;
    PlaceBlockVoxelLocation.X = BlockVoxelLocation.X + HitNormal.X;
    PlaceBlockVoxelLocation.Y = BlockVoxelLocation.Y + HitNormal.Y;
    PlaceBlockVoxelLocation.Z = BlockVoxelLocation.Z + HitNormal.Z;

    FBlockState BlockState = WorldManager->GetBlockState(PlaceBlockVoxelLocation);
    if (!BlockState.IsAir()) return false;

    if (WorldManager->PlaceBlock(PlaceBlockVoxelLocation, FBlockState(Block)))
    {
        Player->ConsumeItem();
        Player->UpdateMainHandItem();

        return true;
    }

    return false;
}
