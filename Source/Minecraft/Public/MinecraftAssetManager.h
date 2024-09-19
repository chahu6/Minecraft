// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MinecraftAssetManager.generated.h"

class UItem;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UMinecraftAssetManager& Get();

	static const FPrimaryAssetType BlockType;
	static const FPrimaryAssetType ItemType;

	/**
	 * Synchronously loads an RPGItem subclass, this can hitch but is useful when you cannot wait for an async load
	 * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
	 *
	 * @param PrimaryAssetId The asset identifier to load
	 * @param bDisplayWarning If true, this will log a warning if the item failed to load
	 */
	UItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);

protected:
	virtual void StartInitialLoading() override;
};
