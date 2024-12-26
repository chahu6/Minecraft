// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * FMinecraftGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
class MINECRAFT_API FMinecraftGameplayTags
{
public:
	static const FMinecraftGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	/*
	* 物品
	*/

	FGameplayTag Air;
	FGameplayTag Item_Block_Stone;
	FGameplayTag Item_Block_Grass;
	FGameplayTag Item_Block_Dirt;
	FGameplayTag Item_Block_Bedrock;
	FGameplayTag Item_Block_Water;
	FGameplayTag Item_Block_Sand;
	FGameplayTag Item_Block_Tallgrass;
	FGameplayTag Item_Block_Rose;
	FGameplayTag Item_Block_Log;
	FGameplayTag Item_Block_Planks;
	FGameplayTag Item_Block_Chest;
	FGameplayTag Item_Block_CraftingTable;
	FGameplayTag Item_Block_Ice;
	FGameplayTag Item_Block_Leaves;

	/*
	* 生物群系
	*/

	FGameplayTag Biome_FlourishIceField;
	FGameplayTag Biome_BarrenIceField;
	FGameplayTag Biome_InlandForest;
	FGameplayTag Biome_InlandPlain;
	FGameplayTag Biome_Desert;
	FGameplayTag Biome_Forest;
	FGameplayTag Biome_RainForest;
	FGameplayTag Biome_Savanna;
	FGameplayTag Biome_Ocean;

private:
	FMinecraftGameplayTags() = default;
	~FMinecraftGameplayTags() = default;

	static FMinecraftGameplayTags GameplayTags;
};

#define AIR FMinecraftGameplayTags::Get().Air