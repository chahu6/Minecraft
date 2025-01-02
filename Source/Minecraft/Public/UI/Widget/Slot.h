// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slot.generated.h"

class UInventoryInterface;
struct FItemStack;

/**
 * 
 */
UCLASS()
class MINECRAFT_API USlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FlushItemData(const FItemStack& ItemStack);

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Index;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TScriptInterface<UInventoryInterface> InventoryInterface;
};
