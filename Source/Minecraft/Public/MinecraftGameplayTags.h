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
	FGameplayTag Minecraft_Dirt;
	FGameplayTag Minecraft_Grass_Block;
	FGameplayTag Minecraft_Grass;
	FGameplayTag Minecraft_Rose;
	FGameplayTag Minecraft_Oak_Wood;
	FGameplayTag Minecraft_Oak_Plants;

private:
	FMinecraftGameplayTags() = default;
	~FMinecraftGameplayTags() = default;

	static FMinecraftGameplayTags GameplayTags;
};
