// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEntity/TileEntity.h"

ATileEntity::ATileEntity()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ATileEntity::BeginPlay()
{
	Super::BeginPlay();

}

void ATileEntity::OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	ReceiveOnBlockActivated(WorldManager, BlockVoxelLoc, Player);
}