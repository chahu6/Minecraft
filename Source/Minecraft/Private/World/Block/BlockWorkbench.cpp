// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Block/BlockWorkbench.h"
#include "Player/EntityPlayer.h"

bool UBlockWorkbench::OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	return Player->DisplayGui(WorkbenchClass);
}
