// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MinecraftAssetLibrary.h"
#include "World/MinecraftSettings.h"
#include "World/Block/Block.h"

const FBlockInfoTableRow& UMinecraftAssetLibrary::GetBlockInfo(int32 BlockID)
{
	const UMinecraftSettings* Setting = GetDefault<UMinecraftSettings>();
	UDataTable* DataTable = Setting->BlockDataTable.Get();
	check(DataTable != nullptr);

	return (*DataTable->FindRow<FBlockInfoTableRow>(FName(FString::FromInt(BlockID)), nullptr));
}
