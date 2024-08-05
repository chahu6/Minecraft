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

protected:
	virtual void BeginPlay() override;

	virtual void NotifyAndUpdateUI() override;

	bool IsHotbarSlot(int32 Index);

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdate OnHotbarUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdate OnArmorUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> ArmorItems;

	UPROPERTY()
	FItemData OffHand;
};
