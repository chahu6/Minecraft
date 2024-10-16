// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEntity/TileEntityChest.h"
#include "Player/EntityPlayer.h"
#include "UI/Widget/Chest.h"
#include "Components/Inventory/InventoryComponent.h"

ATileEntityChest::ATileEntityChest()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void ATileEntityChest::BeginPlay()
{
	Super::BeginPlay();

}

bool ATileEntityChest::OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	if (UChest* Chest = CreateWidget<UChest>(Player->GetController<APlayerController>(), ChestClass))
	{
		Chest->ChestInventory = InventoryComponent;
		return Player->DisplayGui(Chest);
	}

	return false;
}