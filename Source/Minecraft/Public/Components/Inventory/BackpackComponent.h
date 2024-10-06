// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InventoryInterface.h"
#include "BackpackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackpackUpdate);

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBackpackComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:
	UBackpackComponent();

	virtual void BeginPlay() override;

	/** Inventory Interface */
	virtual int32 GetSizeInventory_Implementation() override;
	virtual bool IsEmpty_Implementation() const override;
	virtual bool IsEmptyFromIndex_Implementation(int32 Index) const override;
	virtual FItemStack GetItemStack_Implementation(int32 Index) override;
	virtual FItemStack DecrStackSize_Implementation(int32 Index, int32 Count) override;
	virtual FItemStack RemoveStackFromSlot_Implementation(int32 Index) override;
	virtual void SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack) override;
	virtual bool AddItemToInventoryFromIndex_Implementation(int32 Index, FItemStack& InItemStack);
	virtual void RemoveItemFromInventory_Implementation(int32 Index, FItemStack& InItemStack);
	virtual void Clear_Implementation() override;
	/** end Inventory Interface */

	FItemStack GetHotbarItemStack(int32 SelectedIndex);

	virtual void ConsumeItem(int32 SelectedIndex);

	virtual bool AddItemToInventory(FItemStack& ItemStack);

	FItemStack DecreStackSize(int32 Index, int32 Count);

	FORCEINLINE bool IsValidIndex(int32 Index) const { return Items.IsValidIndex(Index); }

protected:
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
	FOnBackpackUpdate OnHotbarUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnBackpackUpdate OnInventoryUpdate;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStack> Items;

	UPROPERTY(EditAnywhere)
	int32 InventorySize = 36;

	//UPROPERTY()
	//FItemData OffHand;

	/*UPROPERTY()
	FItemData OffHand_Test;*/
};
