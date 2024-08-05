// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Info/ItemInfo.h"
#include "Interfaces/InventoryInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInventoryComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void TransferItem(int32 Index, UPARAM(ref) FItemData& OutItemData);

	/** Inventory Interface */
	virtual void TryAddItem_Implementation(int32 Index, FItemData& InItemData) override;
	virtual void RemoveItem_Implementation(int32 Index, FItemData& OutItemData) override;
	/** end Inventory Interface */

	bool AddItemToInventory(FItemData& ItemData);

protected:
	virtual void BeginPlay() override;

	/*
	* 背包数据更新后所作的事情
	*/
	virtual void AfterDataUpdate();
		
	/*
	* 通知UI更新
	*/
	virtual void NotifyAndUpdateUI();

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdate OnInventoryUpdate;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> Items;

	UPROPERTY(EditAnywhere)
	int32 InventorySize = 9;
};
