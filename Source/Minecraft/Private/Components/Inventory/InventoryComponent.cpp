// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Item/Item.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(InventorySize);
}

void UInventoryComponent::ConsumeItem(int32 SelectedIndex)
{
	if (IsValidIndex(SelectedIndex))
	{
		FItemStack& ItemStack = Items[SelectedIndex];
		ItemStack.Decrement();
	}
}

bool UInventoryComponent::AddItemToInventoryFromIndex(int32 Index, FItemStack& InItemStack)
{
	if (InItemStack.IsEmpty() || !IsValidIndex(Index)) return false;

	FItemStack& ItemStack = Items[Index];

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
					int32 Sum = ItemStack.GetCount() + InItemStack.GetCount();
					if (Sum <= ItemStack.GetMaxStackSize())
					{
						ItemStack.SetCount(Sum);
						InItemStack.Empty();
					}
					else
					{
						ItemStack.SetCount(ItemStack.GetMaxStackSize());
						InItemStack.SetCount(Sum - ItemStack.GetCount());
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
		return Items[Index];
	}
	return FItemStack();
}

void UInventoryComponent::RemoveItemFromInventory(int32 Index, FItemStack& InItemStack)
{
	if (IsValidIndex(Index))
	{
		InItemStack = Items[Index];
		Items[Index].Empty();
	}
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

void UInventoryComponent::DropAllItems()
{
	for (FItemStack& ItemStack : Items)
	{
		if (!ItemStack.IsEmpty())
		{
			//DropItem(ItemStack);
		}
	}
}

bool UInventoryComponent::AddSameItem(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	// ²»¿É¶Ñµþ
	if (!InItemStack.IsStack()) return bFlag;

	for (FItemStack& ItemStack : Items)
	{
		if (ItemStack.IsEmpty() || ItemStack.IsFull()) continue;

		if (InItemStack.GetItem() == ItemStack.GetItem())
		{
			int32 Sum = InItemStack.GetCount() + ItemStack.GetCount();
			if (Sum <= ItemStack.GetMaxStackSize())
			{
				ItemStack.SetCount(Sum);
				InItemStack.Empty();
			}
			else
			{
				ItemStack.SetCount(ItemStack.GetMaxStackSize());
				InItemStack.SetCount(Sum - ItemStack.GetCount());
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

	for (FItemStack& ItemStack : Items)
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
