// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecraftAssetManager.h"
#include "Engine/DataTable.h"
#include "MinecraftGameplayTags.h"
#include "World/MinecraftSettings.h"
#include "Item/Item.h"
#include "World/Biome/Biome.h"

const FPrimaryAssetType UMinecraftAssetManager::BlockType = FName(TEXT("Block"));
const FPrimaryAssetType UMinecraftAssetManager::ItemType = FName(TEXT("Item"));
const FPrimaryAssetType UMinecraftAssetManager::BiomeType = FName(TEXT("Biome"));

UMinecraftAssetManager& UMinecraftAssetManager::Get()
{
	check(GEngine);

	UMinecraftAssetManager* MinecraftAssetManager = Cast<UMinecraftAssetManager>(GEngine->AssetManager);
	return *MinecraftAssetManager;
}

void UMinecraftAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FMinecraftGameplayTags::InitializeNativeGameplayTags();

	const UMinecraftSettings* Setting = GetDefault<UMinecraftSettings>();
	check(Setting);

	Setting->ItemDataTable.LoadSynchronous();
	Setting->BlockDataTable.LoadSynchronous();
}

UItem* UMinecraftAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	UItem* LoadedItem = Cast<UItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
