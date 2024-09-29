// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Item/Item.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItemToInventoryFromIndex(int32 Index, FItemStack& InItemStack)
{
	if (InItemStack.IsEmpty() || !IsValidIndex(Index)) return false;

	FItemStack& ItemStack = Items_Test[Index];

	if (ItemStack.IsEmpty())
	{
		ItemStack = InItemStack;
		InItemStack.Empty();
	}
	else
	{
		if (InItemStack.IsStack())
		{
			if (ItemStack.GetItem() == InItemStack.GetItem())
			{
				if (!ItemStack.IsFull())
				{
					int32 Sum = ItemStack.GetStackSize() + InItemStack.GetStackSize();
					if (Sum <= ItemStack.GetMaxStackSize())
					{
						ItemStack.SetStackSize(Sum);
						InItemStack.Empty();
					}
					else
					{
						ItemStack.SetStackSize(ItemStack.GetMaxStackSize());
						InItemStack.SetStackSize(Sum - ItemStack.GetStackSize());
					}
				}
			}
			else
			{
				FItemStack TempItemStack = ItemStack;
				ItemStack = InItemStack;
				InItemStack = TempItemStack;
			}
		}
		else
		{
			FItemStack TempItemStack = ItemStack;
			ItemStack = InItemStack;
			InItemStack = TempItemStack;
		}
	}

	return InItemStack.IsEmpty();
}

FItemStack UInventoryComponent::GetItemStack(int32 Index) const
{
	if (IsValidIndex(Index))
	{
		return Items_Test[Index];
	}
	return FItemStack();
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(InventorySize);

	Items_Test.SetNum(InventorySize);
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

bool UInventoryComponent::AddItemToInventory(FItemStack& ItemStack)
{
	if (ItemStack.IsEmpty()) return false;

	bool bSameFlag = AddSameItem(ItemStack);

	bool bAddFlag = AddItemStack(ItemStack);

	if (bSameFlag || bAddFlag)
	{
		OnInventoryUpdate.Broadcast();
	}

	return ItemStack.IsEmpty();
}

void UInventoryComponent::AfterDataUpdate()
{
	NotifyAndUpdateUI();
}

void UInventoryComponent::NotifyAndUpdateUI()
{
	OnInventoryUpdate.Broadcast();
}

bool UInventoryComponent::AddSameItem(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	// ²»¿É¶Ñµþ
	if (!InItemStack.IsStack()) return bFlag;

	for (FItemStack& ItemStack : Items_Test)
	{
		if (ItemStack.IsEmpty() || ItemStack.IsFull()) continue;

		if (InItemStack.GetItem() == ItemStack.GetItem())
		{
			int32 Sum = InItemStack.GetStackSize() + ItemStack.GetStackSize();
			if (Sum <= ItemStack.GetMaxStackSize())
			{
				ItemStack.SetStackSize(Sum);
				InItemStack.Empty();
			}
			else
			{
				ItemStack.SetStackSize(ItemStack.GetMaxStackSize());
				InItemStack.SetStackSize(Sum - ItemStack.GetStackSize());
			}
			bFlag = true;
		}
	}

	return bFlag;
}

bool UInventoryComponent::AddItemStack(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	for (FItemStack& ItemStack : Items_Test)
	{
		if (ItemStack.IsEmpty())
		{
			ItemStack = InItemStack;
			InItemStack.Empty();
			bFlag = true;
			break;
		}
	}

	return bFlag;
}
