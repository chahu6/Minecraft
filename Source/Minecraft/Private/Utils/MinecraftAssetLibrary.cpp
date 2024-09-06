// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MinecraftAssetLibrary.h"
#include "World/MinecraftSettings.h"
#include "World/Block/Block.h"
#include "Item/ItemInfo.h"

bool UMinecraftAssetLibrary::GetBlockMeta(int32 BlockID, FBlockMeta& OutBlockInfo)
{
	const UMinecraftSettings* Setting = GetDefault<UMinecraftSettings>();
	UDataTable* DataTable = Setting->BlockDataTable.Get();
	check(DataTable != nullptr);

	FBlockMeta* Info = DataTable->FindRow<FBlockMeta>(FName(FString::FromInt(BlockID)), nullptr);
	if (Info)
	{
		OutBlockInfo = *Info;
		return true;
	}

	return false;
}

bool UMinecraftAssetLibrary::GetItemInstance(int32 ItemID, FItemInstance& OutItemInstance)
{
	const UMinecraftSettings* Setting = GetDefault<UMinecraftSettings>();
	UDataTable* DataTable = Setting->ItemDataTable.Get();
	check(DataTable != nullptr);

	FItemInstance* ItemInstance = DataTable->FindRow<FItemInstance>(FName(FString::FromInt(ItemID)), nullptr);
	if (ItemInstance)
	{
		OutItemInstance = *ItemInstance;
		return true;
	}

	return false;
}
