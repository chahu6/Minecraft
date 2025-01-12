// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStack.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "InventoryComponent.generated.h"

/**
* 
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInventoryComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

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

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdateSignature OnInventoryItemUpdateDelegate;

protected:
	FORCEINLINE bool IsValidIndex(int32 Index) const { return Items.IsValidIndex(Index); }

	virtual void Notify(int32 Index);

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStack> Items;

	UPROPERTY(EditAnywhere)
	int32 InventorySize = 27;
};
