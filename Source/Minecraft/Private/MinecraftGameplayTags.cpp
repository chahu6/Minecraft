// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecraftGameplayTags.h"
#include "GameplayTagsManager.h"

FMinecraftGameplayTags FMinecraftGameplayTags::GameplayTags;

void FMinecraftGameplayTags::InitializeNativeGameplayTags()
{
	/*
	* 物品
	*/

	GameplayTags.Minecraft_Air = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Air")
	);

	GameplayTags.Minecraft_Stone = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Stone")
	);

	GameplayTags.Minecraft_Dirt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Dirt")
	);

	GameplayTags.Minecraft_Grass = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Grass")
	);

	GameplayTags.Minecraft_Bedrock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Bedrock")
	);

	GameplayTags.Minecraft_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Water")
	);

	GameplayTags.Minecraft_Sand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Sand")
	);

	GameplayTags.Minecraft_Tallgrass = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Tallgrass")
	);

	GameplayTags.Minecraft_Rose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Rose")
	);

	GameplayTags.Minecraft_Log = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Log")
	);

	GameplayTags.Minecraft_Planks = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Planks")
	);

	GameplayTags.Minecraft_Chest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Chest")
	);

	GameplayTags.Minecraft_Crafting_Table = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Crafting_Table")
	);

	GameplayTags.Minecraft_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Ice")
	);

	/*
	* 生物群系
	*/

	GameplayTags.Biomes_FlourishIceField = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.FlourishIceField")
	);
	GameplayTags.Biomes_BarrenIceField = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.BarrenIceField")
	);
	GameplayTags.Biomes_InlandForest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.InlandForest")
	);
	GameplayTags.Biomes_InlandPlain = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.InlandPlain")
	);
	GameplayTags.Biomes_Desert = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.Desert")
	);
	GameplayTags.Biomes_Forest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.Forest")
	);
	GameplayTags.Biomes_RainForest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.RainForest")
	); 
	GameplayTags.Biomes_Savanna = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.Savanna")
	);
	GameplayTags.Biomes_Ocean = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Biomes.Ocean")
	);
}
