// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slot.generated.h"

class UInventoryInterface;
struct FItemStack;
class UContainer;

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsItemValid(const FItemStack& ItemStack) const;
	virtual bool IsItemValid_Implementation(const FItemStack& ItemStack) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnTake();
	virtual void OnTake_Implementation();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UContainer> Parent;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Index;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TScriptInterface<UInventoryInterface> InventoryInterface;

};
