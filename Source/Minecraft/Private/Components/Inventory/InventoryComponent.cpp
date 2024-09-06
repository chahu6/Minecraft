// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(InventorySize);
}

void UInventoryComponent::TryAddItem_Implementation(int32 Index, FItemData& InItemData)
{
	if (Items.IsValidIndex(Index) && InItemData.IsValid())
	{
		FItemData& ItemData = Items[Index];
		if (ItemData.ID > 0 && ItemData.ID == InItemData.ID && ItemData.bIsStack)
		{
			int32 Sum = ItemData.Quantity + InItemData.Quantity;
			if (ItemData.MaxCount >= Sum)
			{
				ItemData.Quantity = Sum;
				InItemData.Clear();
			}
			else
			{
				ItemData.Quantity = ItemData.MaxCount;
				InItemData.Quantity = Sum - ItemData.MaxCount;
			}
		}
		else if(ItemData.ID > 0)
		{
			FItemData TempItemData = ItemData;
			ItemData = InItemData;
			InItemData = TempItemData;
		}
		else
		{
			ItemData = InItemData;
			InItemData.Clear();
		}

		AfterDataUpdate();
	}
}

void UInventoryComponent::RemoveItem_Implementation(int32 Index, FItemData& OutItemData)
{
	if (Items.IsValidIndex(Index))
	{
		OutItemData = Items[Index];
		Items[Index].Clear();

		AfterDataUpdate();
	}
	else
	{
		OutItemData.Clear();
	}
}

void UInventoryComponent::TransferItem_Implementation(int32 Index, FItemData& OutItemData)
{
	if (Items.IsValidIndex(Index))
	{
		FItemData TempItemData = OutItemData;
		OutItemData = Items[Index];
		Items[Index] = TempItemData;
		AfterDataUpdate();
	}
}

bool UInventoryComponent::AddItemToInventory(FItemData& ItemData)
{
	if (!ItemData.IsValid()) return false;

	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		if (Items[Index].ID == ItemData.ID && ItemData.IsValid() && ItemData.bIsStack && Items[Index].Quantity < ItemData.MaxCount)
		{
			Execute_TryAddItem(this, Index, ItemData);
		}
	}

	if (ItemData.IsValid())
	{
		for (int32 Index = 0; Index < Items.Num(); ++Index)
		{
			if (Items[Index].ID == 0)
			{
				Execute_TryAddItem(this, Index, ItemData);
			}
		}
	}

	return !ItemData.IsValid();
}

void UInventoryComponent::AfterDataUpdate()
{
	NotifyAndUpdateUI();
}

void UInventoryComponent::NotifyAndUpdateUI()
{
	OnInventoryUpdate.Broadcast();
}
