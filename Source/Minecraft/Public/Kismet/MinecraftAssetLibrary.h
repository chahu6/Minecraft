// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinecraftAssetLibrary.generated.h"

struct FBlockMeta;
struct FItemInstance;
class UItem;
struct FItemStack;

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

	/** 
	* Item
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	static bool IsEmpty(const FItemStack& ItemStack);

	UFUNCTION(BlueprintCallable, Category = "Item")
	static void ClearItemStack(UPARAM(ref) FItemStack& ItemStack);
};
