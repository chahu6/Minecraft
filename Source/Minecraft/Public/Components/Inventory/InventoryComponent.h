// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemStack.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);

class AMinecraftPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	bool AddItemToInventory(FItemStack& ItemStack);

	UFUNCTION(BlueprintCallable)
	bool TransferSlots(FItemStack HangItemStack, FItemStack& NewHangItemStack, UInventoryComponent* DestinationInventory, int32 DestinationIndex);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(int32 Index);

	FItemStack GetItemStack(int32 Index) const;
	bool SetItemStack(int32 Index, const FItemStack& NewItemStack);

protected:
	virtual void BeginPlay() override;

	bool IsValidIndex(int32 Index) const;

	/*
	* 背包数据更新后所作的事情
	*/
	virtual void AfterDataUpdate(int32 Index);
		
	/*
	* 通知UI更新
	*/
	virtual void NotifyAndUpdateUI(int32 Index);

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryUpdate;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> ItemsData;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventorySize = 9;
};
