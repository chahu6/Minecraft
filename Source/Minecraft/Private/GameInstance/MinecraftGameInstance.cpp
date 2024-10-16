// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MinecraftGameInstance.h"
#include "MinecraftAssetManager.h"
#include "World/Block/Block.h"
#include "Item/Item.h"
#include "Init/Items.h"
#include "Init/Blocks.h"
#include "Init/Biomes.h"
#include "Item/Crafting/CraftingManager.h"
#include "World/Biome/Biome.h"

void UMinecraftGameInstance::Init()
{
	Super::Init();

	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<FPrimaryAssetId> BlockIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::BlockType, BlockIdList);

	TArray<FPrimaryAssetId> ItemIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::ItemType, ItemIdList);
	
	ItemIdList.Append(BlockIdList);

	LoadAssetHandle = AssetManager.LoadPrimaryAssets(ItemIdList, TArray<FName>(), FStreamableDelegate());

	EAsyncPackageState::Type Result = LoadAssetHandle->WaitUntilComplete();

	check(Result == EAsyncPackageState::Complete);

	UBlock::RegisterBlocks();
	UItem::RegisterItems();
	UBiome::RegisterBiomes();

	UBlocks::Initialization();
	UItems::Initialization();
	UBiomes::Initialization();

	CraftingManager::Init();
}
