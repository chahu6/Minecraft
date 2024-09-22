// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStack.generated.h"

/**
 * 
 */
USTRUCT()
struct MINECRAFT_API FItemStack final
{
	GENERATED_USTRUCT_BODY()
public:
	FItemStack();

private:
	int32 StackSize;
};
