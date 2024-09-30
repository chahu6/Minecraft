// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemStack.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void ConsumeItem(int32 SelectedIndex);

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual bool AddItemToInventory(UPARAM(ref) FItemStack& ItemStack);

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual bool AddItemToInventoryFromIndex(int32 Index, UPARAM(ref) FItemStack& InItemStack);

	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemStack GetItemStack(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void RemoveItemFromInventory(int32 Index, UPARAM(ref) FItemStack& InItemStack);

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void DropAllItems();

protected:
	bool AddSameItem(FItemStack& InItemStack);

	bool AddItemStack(FItemStack& InItemStack);

	FORCEINLINE bool IsValidIndex(int32 Index) const { return Items.IsValidIndex(Index); }

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdate OnInventoryUpdate;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStack> Items;

	UPROPERTY(EditAnywhere)
	int32 InventorySize = 9;
};
