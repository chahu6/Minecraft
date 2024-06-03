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

private:
	bool IsHotbarSlot(int32 Index);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> ArmorItems;

	FItemStack OffHand;

	int32 HotbarSelectedIndex = 0;
};
