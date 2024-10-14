// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Block/BlockWorkbench.h"
#include "Player/EntityPlayer.h"
#include "UI/Widget/Container.h"

bool UBlockWorkbench::OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	return Player->DisplayGui(CreateWidget<UContainer>(Player->GetController<APlayerController>(), WorkbenchClass));
}
