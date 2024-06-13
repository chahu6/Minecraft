// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecraftAssetManager.h"
#include "World/MinecraftSettings.h"
#include "Engine/DataTable.h"
#include "Item/Items.h"

UMinecraftAssetManager& UMinecraftAssetManager::Get()
{
	check(GEngine);

	UMinecraftAssetManager* MinecraftAssetManager = Cast<UMinecraftAssetManager>(GEngine->AssetManager);
	return *MinecraftAssetManager;
}

void UMinecraftAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	const UMinecraftSettings* Setting = GetDefault<UMinecraftSettings>();
	check(Setting);

	Setting->BlockDataTable.LoadSynchronous();
	Setting->ItemDataTable.LoadSynchronous();

	// ≥ı ºªØ
	UItems::InitializeItems();
}
