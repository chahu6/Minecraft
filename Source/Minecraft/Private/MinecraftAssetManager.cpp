// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecraftAssetManager.h"
#include "World/MinecraftSettings.h"
#include "Engine/DataTable.h"
#include "MinecraftGameplayTags.h"
#include "Components/Crafting/CraftingComponent.h"

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

	UCraftingComponent::InitialItemRecipes();

	const UMinecraftSettings* Setting = GetDefault<UMinecraftSettings>();
	check(Setting);

	Setting->ItemDataTable.LoadSynchronous();
	Setting->BlockDataTable.LoadSynchronous();
}
