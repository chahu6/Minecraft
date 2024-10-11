// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Inventory.h"
#include "Workbench.generated.h"

class UCraftingComponent;
class UBackpackComponent;
class UCraftingResultComponent;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UWorkbench : public UInventory
{
	GENERATED_BODY()
protected:
	UWorkbench();

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void FlushCrafting();

	UFUNCTION(BlueprintImplementableEvent)
	void FlushCraftingResult();

	virtual void OnCraftMatrixChanged() override;

	virtual void OnHandleLMB(UInventoryItem* InventoryItem) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingComponent> CraftingComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingResultComponent> CraftingResultComponent;
};
