// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryComponent.h"
#include "BackpackComponent.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBackpackComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	UBackpackComponent();

	FItemStack GetHotbarItemStack(int32 SelectedIndex);

	virtual void ConsumeItem(int32 SelectedIndex) override;

	virtual bool AddItemToInventory(FItemStack& ItemStack) override;

	virtual bool AddItemToInventoryFromIndex(int32 Index, FItemStack& InItemStack) override;

	virtual void RemoveItemFromInventory(int32 Index, FItemStack& InItemStack) override;

	FItemStack DecreStackSize(int32 Index, int32 Count);

protected:
	virtual void BeginPlay() override;

	bool AddItemToBackpack(FItemStack& ItemStack);
	bool AddSameItemToBackpack(FItemStack& InItemStack);
	bool AddItemStackBackpack(FItemStack& InItemStack);

	bool AddItemToHotbar(FItemStack& ItemStack);
	bool AddSameItemToHotbar(FItemStack& InItemStack);
	bool AddItemStackHotbar(FItemStack& InItemStack);

	bool IsHotbarIndex(int32 Index);

	void NotifyAndUpdateUI(int32 Index);

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdate OnHotbarUpdate;

private:
	//UPROPERTY()
	//FItemData OffHand;

	/*UPROPERTY()
	FItemData OffHand_Test;*/
};
