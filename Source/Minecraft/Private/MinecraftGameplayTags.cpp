// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecraftGameplayTags.h"
#include "GameplayTagsManager.h"

FMinecraftGameplayTags FMinecraftGameplayTags::GameplayTags;

void FMinecraftGameplayTags::InitializeNativeGameplayTags()
{
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
}
