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

	GameplayTags.Minecraft_Grass_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Grass_Block")
	);

	GameplayTags.Minecraft_Rose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Rose")
	);

	GameplayTags.Minecraft_Oak_Wood = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Oak_Wood")
	);

	GameplayTags.Minecraft_Oak_Plants = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Minecraft.Oak_Plants")
	);
}
