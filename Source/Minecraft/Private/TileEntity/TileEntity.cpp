// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEntity/TileEntity.h"

ATileEntity::ATileEntity()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool ATileEntity::OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	return false;
}

void ATileEntity::BeginPlay()
{
	Super::BeginPlay();

}