// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBlock.h"
#include "Item/ItemStack.h"
#include "World/WorldManager.h"
#include "Player/EntityPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "World/Block/Block.h"
#include "World/WorldGenerator.h"

UBlock* UItemBlock::GetBlock() const
{
    return Block;
}

bool UItemBlock::OnItemUse(AEntityPlayer* Player, AWorldManager* WorldManager, const FIntVector& BlockVoxelLocation, const FVector& HitNormal)
{
    FIntVector PlaceBlockVoxelLocation;
    PlaceBlockVoxelLocation.X = BlockVoxelLocation.X + HitNormal.X;
    PlaceBlockVoxelLocation.Y = BlockVoxelLocation.Y + HitNormal.Y;
    PlaceBlockVoxelLocation.Z = BlockVoxelLocation.Z + HitNormal.Z;

    FBlockState BlockState = WorldManager->GetBlockState(PlaceBlockVoxelLocation);
    if (!BlockState.IsAir()) return false;

    WorldManager->PlaceBlock(PlaceBlockVoxelLocation, FBlockState(Block));

    FVector WorldLocation = FVector(PlaceBlockVoxelLocation * WorldGenerator::BlockSize);
    WorldLocation = WorldLocation + (WorldGenerator::BlockSize >> 1);

    UGameplayStatics::PlaySoundAtLocation(WorldManager, Block->PlaceSound, WorldLocation);

    Player->ConsumeItem();
    Player->UpdateMainHandItem();

    return true;
}
