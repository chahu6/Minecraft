// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

using item_t = uint16;
using biome_t = uint8;

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

	TMap<FGameplayTag, item_t> ItemLookupTable;
	TMap<item_t, FGameplayTag> ReverseItemLookupTable;

	/*
	* 生物群系
	*/

	FGameplayTag Biome_Ocean;
	FGameplayTag Biome_FlourishIceField;
	FGameplayTag Biome_BarrenIceField;
	FGameplayTag Biome_InlandForest;
	FGameplayTag Biome_InlandPlain;
	FGameplayTag Biome_Desert;
	FGameplayTag Biome_Forest;
	FGameplayTag Biome_RainForest;
	FGameplayTag Biome_Savanna;

	TMap<FGameplayTag, biome_t> BiomeLookupTable;
	TMap<biome_t, FGameplayTag> ReverseBiomeLookupTable;

private:
	FMinecraftGameplayTags() = default;
	~FMinecraftGameplayTags() = default;

	static FMinecraftGameplayTags GameplayTags;
};

#define AIR FMinecraftGameplayTags::Get().Air