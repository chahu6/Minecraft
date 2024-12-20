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

	FGameplayTag Minecraft_Air;
	FGameplayTag Minecraft_Stone;
	FGameplayTag Minecraft_Grass;
	FGameplayTag Minecraft_Dirt;
	FGameplayTag Minecraft_Bedrock;
	FGameplayTag Minecraft_Water;
	FGameplayTag Minecraft_Sand;
	FGameplayTag Minecraft_Tallgrass;
	FGameplayTag Minecraft_Rose;
	FGameplayTag Minecraft_Log;
	FGameplayTag Minecraft_Planks;
	FGameplayTag Minecraft_Chest;
	FGameplayTag Minecraft_Crafting_Table;
	FGameplayTag Minecraft_Ice;

	/*
	* 生物群系
	*/

	FGameplayTag Biomes_FlourishIceField;
	FGameplayTag Biomes_BarrenIceField;
	FGameplayTag Biomes_InlandForest;
	FGameplayTag Biomes_InlandPlain;
	FGameplayTag Biomes_Desert;
	FGameplayTag Biomes_Forest;
	FGameplayTag Biomes_RainForest;
	FGameplayTag Biomes_Savanna;
	FGameplayTag Biomes_Ocean;

private:
	FMinecraftGameplayTags() = default;
	~FMinecraftGameplayTags() = default;

	static FMinecraftGameplayTags GameplayTags;
};
