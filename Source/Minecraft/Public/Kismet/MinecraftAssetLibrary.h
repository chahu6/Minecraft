// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinecraftAssetLibrary.generated.h"

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
	/** 
	* Item
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	static bool IsEmpty(const FItemStack& ItemStack);

	UFUNCTION(BlueprintCallable, Category = "Item")
	static void ClearItemStack(UPARAM(ref) FItemStack& ItemStack);

	UFUNCTION(BlueprintPure, Category = "Item")
	static UItem* GetItemFromID(const FGameplayTag& InItemID);
};
