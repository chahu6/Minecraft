// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MinecraftGameInstance.h"
#include "MinecraftAssetManager.h"
#include "World/Block/Blocks.h"
#include "World/WorldManager.h"

void UMinecraftGameInstance::Init()
{
	Super::Init();

	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<FPrimaryAssetId> BlockIdList;
	AssetManager.GetPrimaryAssetIdList(UMinecraftAssetManager::BlockType, BlockIdList);
	
	AssetManager.LoadPrimaryAssets(BlockIdList, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UMinecraftGameInstance::CallbackFunction));
}

void UMinecraftGameInstance::CallbackFunction()
{
	UBlock::Initializer();
	UBlocks::Initializer();

	GetWorld()->SpawnActor<AWorldManager>(WorldManagerClass);
}
