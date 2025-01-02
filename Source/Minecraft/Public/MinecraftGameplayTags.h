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
	FGameplayTag Stone;
	FGameplayTag Grass;
	FGameplayTag Dirt;
	FGameplayTag Bedrock;
	FGameplayTag Water;
	FGameplayTag Sand;
	FGameplayTag Tallgrass;
	FGameplayTag Rose;
	FGameplayTag Log;
	FGameplayTag Planks;
	FGameplayTag Chest;
	FGameplayTag CraftingTable;
	FGameplayTag Ice;
	FGameplayTag Leaves;

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