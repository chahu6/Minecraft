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

	GameplayTags.Item_Block_Stone = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Stone")
	);

	GameplayTags.Item_Block_Dirt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Dirt")
	);

	GameplayTags.Item_Block_Grass = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Grass")
	);

	GameplayTags.Item_Block_Bedrock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Bedrock")
	);

	GameplayTags.Item_Block_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Water")
	);

	GameplayTags.Item_Block_Sand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Sand")
	);

	GameplayTags.Item_Block_Tallgrass = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Tallgrass")
	);

	GameplayTags.Item_Block_Rose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Rose")
	);

	GameplayTags.Item_Block_Log = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Log")
	);

	GameplayTags.Item_Block_Planks = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Planks")
	);

	GameplayTags.Item_Block_Chest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Chest")
	);

	GameplayTags.Item_Block_CraftingTable = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.CraftingTable")
	);

	GameplayTags.Item_Block_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Ice")
	);

	GameplayTags.Item_Block_Leaves = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Block.Leaves")
	);

	GameplayTags.ItemLookupTable.Add(GameplayTags.Air, 0);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Stone, 1);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Grass, 2);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Dirt, 3);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Bedrock, 4);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Water, 5);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Sand, 6);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Tallgrass, 7);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Rose, 8);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Log, 9);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Planks, 10);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Chest, 11);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_CraftingTable, 12);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Ice, 13);
	GameplayTags.ItemLookupTable.Add(GameplayTags.Item_Block_Leaves, 14);

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
