// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEntity/TileEntityChest.h"
#include "Player/EntityPlayer.h"
#include "Components/Inventory/InventoryComponent.h"
#include "UI/Widget/Container.h"

ATileEntityChest::ATileEntityChest()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void ATileEntityChest::BeginPlay()
{
	Super::BeginPlay();

}

void ATileEntityChest::OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	Super::OnBlockActivated(WorldManager, BlockVoxelLoc, Player);

	ensure(ChestClass);
	Player->DisplayGUI(ChestClass, this);
}