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
	FGameplayTag Minecraft_Crafting_Table;

private:
	FMinecraftGameplayTags() = default;
	~FMinecraftGameplayTags() = default;

	static FMinecraftGameplayTags GameplayTags;
};
