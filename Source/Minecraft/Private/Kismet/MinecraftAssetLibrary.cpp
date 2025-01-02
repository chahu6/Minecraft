// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/MinecraftAssetLibrary.h"
#include "World/MinecraftSettings.h"
#include "Item/ItemStack.h"
#include "Item/Item.h"

bool UMinecraftAssetLibrary::IsEmpty(const FItemStack& ItemStack)
{
	return ItemStack.IsEmpty();
}

UItem* UMinecraftAssetLibrary::GetItemFromID(const FGameplayTag& InItemID)
{
	return UItem::GetItemFromID(InItemID);
}

void UMinecraftAssetLibrary::ClearItemStack(FItemStack& ItemStack)
{
	ItemStack.Empty();
}
