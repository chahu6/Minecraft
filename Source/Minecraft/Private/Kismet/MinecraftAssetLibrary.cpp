// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/MinecraftAssetLibrary.h"
#include "World/MinecraftSettings.h"
#include "Item/ItemStack.h"

bool UMinecraftAssetLibrary::IsEmpty(const FItemStack& ItemStack)
{
	return ItemStack.IsEmpty();
}

void UMinecraftAssetLibrary::ClearItemStack(FItemStack& ItemStack)
{
	ItemStack.Empty();
}
