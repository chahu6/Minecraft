// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MinecraftSubsystem.h"
#include "MinecraftAssetManager.h"
#include "World/Block/Block.h"
#include "Item/Item.h"
#include "Init/Items.h"
#include "Init/Blocks.h"
#include "Init/Biomes.h"
#include "Item/Crafting/CraftingManager.h"
#include "World/Biome/Biome.h"

void UMinecraftSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LoadMinecraftAssets();

	UBlock::RegisterBlocks();
	UItem::RegisterItems();
	UBiome::RegisterBiomes();

	UBlocks::Initialization();
	UItems::Initialization();
	UBiomes::Initialization();

	FCraftingManager::Init();
}

void UMinecraftSubsystem::LoadMinecraftAssets() const
{
	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<FPrimaryAssetId> BlockIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::BlockType, BlockIdList);

	TArray<FPrimaryAssetId> ItemIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::ItemType, ItemIdList);

	TArray<FPrimaryAssetId> BiomeIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::BiomeType, BiomeIdList);

	ItemIdList.Append(BlockIdList);
	ItemIdList.Append(BiomeIdList);

	TSharedPtr<FStreamableHandle> LoadAssetHandle = AssetManager.LoadPrimaryAssets(ItemIdList, TArray<FName>(), FStreamableDelegate());

	EAsyncPackageState::Type Result = LoadAssetHandle->WaitUntilComplete();

	check(Result == EAsyncPackageState::Complete);
}
