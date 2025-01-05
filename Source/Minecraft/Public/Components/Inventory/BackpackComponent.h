// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InventoryInterface.h"
#include "BackpackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemUpdateDelegate, int32, const FItemStack&);

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
	virtual int32 GetSizeInventory_Implementation() const override;
	virtual bool IsEmpty_Implementation() const override;
	virtual bool IsEmptyFromIndex_Implementation(int32 Index) const override;
	virtual FItemStack GetItemStack_Implementation(int32 Index) const override;
	virtual FItemStack DecrStackSize_Implementation(int32 Index, int32 Count) override;
	virtual FItemStack RemoveStackFromSlot_Implementation(int32 Index) override;
	virtual void SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack) override;
	virtual void Clear_Implementation() override;
	/** end Inventory Interface */

	FItemStack GetHotbarItemStack(int32 SelectedIndex);

	virtual void ConsumeItem(int32 SelectedIndex);

	FORCEINLINE bool IsValidIndex(int32 Index) const { return Items.IsValidIndex(Index); }

	
	/*
	* ÐÂµÄº¯Êý
	*/

	int32 GetInventorySize() const;

	FItemStack GetItemStack(int32 Index);

	bool IsEmpty(int32 Index) const;

	void SetHangItemStack(const FItemStack& NewItemStack);

	FOnItemUpdateDelegate OnInventoryItemUpdateDelegate;

protected:
	bool IsHotbarIndex(int32 Index);

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStack> Items;

	UPROPERTY(BlueprintReadOnly)
	FItemStack HangItemStack;

	UPROPERTY(EditAnywhere)
	int32 InventorySize = 36;

	//UPROPERTY()
	//FItemData OffHand;

	/*UPROPERTY()
	FItemData OffHand_Test;*/

public:
	FORCEINLINE FItemStack GetHangItemStack() { return HangItemStack; }
};
