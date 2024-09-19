// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinecraftAssetLibrary.generated.h"

struct FBlockMeta;
struct FItemInstance;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "MinecraftAssetLibrary|Block")
	static bool GetBlockMeta(int32 BlockID, FBlockMeta& OutBlockInfo);

	UFUNCTION(BlueprintCallable, Category = "MinecraftAssetLibrary|Item")
	static bool GetItemInstance(int32 ItemID, FItemInstance& OutItemInstance);
};
