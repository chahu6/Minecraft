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

	FItemStack GetSelected(int32 SelectedIndex);

protected:
	virtual void BeginPlay() override;

protected:
	/*
	* 背包数据更新后所作的事情
	*/
	virtual void AfterDataUpdate(int32 Index) override;

	/*
	* 通知UI更新
	*/
	virtual void NotifyAndUpdateUI(int32 Index) override;

	bool IsHotbarSlot(int32 Index);

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnHotbarUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnArmorUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> ArmorItems;

	FItemStack OffHand;

	int32 HotbarSelectedIndex = 0;
};
