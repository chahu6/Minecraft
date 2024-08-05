// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MinecraftAssetLibrary.h"
#include "World/MinecraftSettings.h"
#include "World/Block/Block.h"

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
