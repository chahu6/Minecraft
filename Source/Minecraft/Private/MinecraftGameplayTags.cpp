// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecraftGameplayTags.h"
#include "GameplayTagsManager.h"

FMinecraftGameplayTags FMinecraftGameplayTags::GameplayTags;

void FMinecraftGameplayTags::InitializeNativeGameplayTags()
{
	/*
	* 物品
	*/

	GameplayTags.Air = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Air")
	);

	GameplayTags.Stone = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Stone")
	);

	GameplayTags.Dirt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Dirt")
	);

	GameplayTags.Grass = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Grass")
	);

	GameplayTags.Bedrock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Bedrock")
	);

	GameplayTags.Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Water")
	);

	GameplayTags.Sand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Sand")
	);

	GameplayTags.Tallgrass = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Tallgrass")
	);

	GameplayTags.Rose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Rose")
	);

	GameplayTags.Log = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Log")
	);

	GameplayTags.Planks = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Planks")
	);

	GameplayTags.Chest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Chest")
	);

	GameplayTags.Workbench = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Workbench")
	);

	GameplayTags.Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Ice")
	);

	GameplayTags.Leaves = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Leaves")
	);

	GameplayTags.ItemLookupTable.Add(GameplayTags.Air, 0);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Stone, 1);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Grass, 2);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Dirt, 3);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Bedrock, 4);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Water, 5);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Sand, 6);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Tallgrass, 7);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Rose, 8);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Log, 9);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Planks, 10);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Chest, 11);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Workbench, 12);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Ice, 13);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Leaves, 14);

	for (const TPair<FGameplayTag, item_t>& Pair : GameplayTags.ItemLookupTable)
	{
		GameplayTags.ReverseItemLookupTable.Add(Pair.Value, Pair.Key);
	}

	/*
	* 生物群系
	*/

	GameplayTags.Biome_FlourishIceField = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.FlourishIceField")
	);
	GameplayTags.Biome_BarrenIceField = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.BarrenIceField")
	);
	GameplayTags.Biome_InlandForest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.InlandForest")
	);
	GameplayTags.Biome_InlandPlain = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.InlandPlain")
	);
	GameplayTags.Biome_Desert = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.Desert")
	);
	GameplayTags.Biome_Forest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.Forest")
	);
	GameplayTags.Biome_RainForest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.RainForest")
	); 
	GameplayTags.Biome_Savanna = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.Savanna")
	);
	GameplayTags.Biome_Ocean = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biome.Ocean")
	);

	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_Ocean, 0);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_FlourishIceField, 1);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_BarrenIceField, 2);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_InlandForest, 3);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_InlandPlain, 4);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_Desert, 5);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_Forest, 6);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_RainForest, 7);
	GameplayTags.BiomeLookupTable.Add(GameplayTags.Biome_Savanna, 8);

	for (const TPair<FGameplayTag, biome_t>& Pair : GameplayTags.BiomeLookupTable)
	{
		GameplayTags.ReverseBiomeLookupTable.Add(Pair.Value, Pair.Key);
	}
}
