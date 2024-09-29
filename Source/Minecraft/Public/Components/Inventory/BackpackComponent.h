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

	FItemData GetSelected(int32 SelectedIndex);

	void ConsumeItem(int32 SelectedIndex);

	virtual bool AddItemToInventory(FItemData& ItemData) override;

	virtual bool AddItemToInventory(FItemStack& ItemStack) override;

	virtual bool AddItemToInventoryFromIndex(int32 Index, FItemStack& InItemStack) override;

protected:
	virtual void BeginPlay() override;

	bool AddItemToBackpack(FItemStack& ItemStack);
	bool AddSameItemToBackpack(FItemStack& InItemStack);
	bool AddItemStackBackpack(FItemStack& InItemStack);

	bool AddItemToHotbar(FItemStack& ItemStack);
	bool AddSameItemToHotbar(FItemStack& InItemStack);
	bool AddItemStackHotbar(FItemStack& InItemStack);

	bool IsHotbarIndex(int32 Index);

	virtual void NotifyAndUpdateUI() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdate OnHotbarUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdate OnArmorUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> ArmorItems;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> ArmorItems_Test;

	UPROPERTY()
	FItemData OffHand;

	UPROPERTY()
	FItemData OffHand_Test;
};
