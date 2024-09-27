// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MinecraftGameInstance.h"
#include "MinecraftAssetManager.h"
#include "World/WorldManager.h"
#include "World/Block/Block.h"
#include "Item/Item.h"
#include "Item/Items.h"
#include "World/Block/Blocks.h"

void UMinecraftGameInstance::Init()
{
	Super::Init();

	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<FPrimaryAssetId> BlockIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::BlockType, BlockIdList);

	TArray<FPrimaryAssetId> ItemIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::ItemType, ItemIdList);
	
	ItemIdList.Append(BlockIdList);
	AssetManager.LoadPrimaryAssets(ItemIdList, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UMinecraftGameInstance::CallbackFunction));
}

void UMinecraftGameInstance::CallbackFunction()
{
	UBlock::RegisterBlocks();
	UItem::RegisterItems();

	UBlocks::Initialization();
	UItems::Initialization();
	
	GetWorld()->SpawnActor<AWorldManager>(WorldManagerClass);
}
