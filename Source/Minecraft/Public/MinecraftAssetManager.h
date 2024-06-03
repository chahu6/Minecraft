// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MinecraftAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UMinecraftAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
