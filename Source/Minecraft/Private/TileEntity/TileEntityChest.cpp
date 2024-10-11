// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEntity/TileEntityChest.h"

ATileEntityChest::ATileEntityChest()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

void ATileEntityChest::BeginPlay()
{
	Super::BeginPlay();

}
