// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinecraftAssetLibrary.generated.h"

struct FBlockInfoTableRow;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "MinecraftAssetLibrary|Block")
	static const FBlockInfoTableRow& GetBlockInfo(int32 BlockID);
};
