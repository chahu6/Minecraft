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
}
