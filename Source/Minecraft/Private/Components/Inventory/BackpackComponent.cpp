// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/BackpackComponent.h"

UBackpackComponent::UBackpackComponent()
{
	InventorySize = 36;
}

void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

}

FItemStack UBackpackComponent::GetHotbarItemStack(int32 SelectedIndex)
{
	if (IsHotbarIndex(SelectedIndex))
	{
		return Items[SelectedIndex];
	}
		
	return {};
}

void UBackpackComponent::ConsumeItem(int32 SelectedIndex)
{
	if (!IsValidIndex(SelectedIndex)) return;

	Super::ConsumeItem(SelectedIndex);

	NotifyAndUpdateUI(SelectedIndex);
}

bool UBackpackComponent::AddItemToInventory(FItemStack& ItemStack)
{
	bool bHotbar = AddItemToHotbar(ItemStack);
	bool bBackpack = AddItemToBackpack(ItemStack);
	return bHotbar || bBackpack;
}

bool UBackpackComponent::AddItemToInventoryFromIndex(int32 Index, FItemStack& InItemStack)
{
	if (!IsValidIndex(Index) || InItemStack.IsEmpty()) return false;

	bool bFlag = Super::AddItemToInventoryFromIndex(Index, InItemStack);

	NotifyAndUpdateUI(Index);

	return bFlag;
}

void UBackpackComponent::RemoveItemFromInventory(int32 Index, FItemStack& InItemStack)
{
	Super::RemoveItemFromInventory(Index, InItemStack);

	NotifyAndUpdateUI(Index);
}

bool UBackpackComponent::AddItemToBackpack(FItemStack& ItemStack)
{
	if (ItemStack.IsEmpty()) return false;

	bool bSameFlag = AddSameItemToBackpack(ItemStack);

	bool bAddFlag = AddItemStackBackpack(ItemStack);

	if (bSameFlag || bAddFlag)
	{
		OnInventoryUpdate.Broadcast();
	}

	return ItemStack.IsEmpty();
}

bool UBackpackComponent::AddSameItemToBackpack(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	if (!InItemStack.IsStack()) return bFlag;	// 不可堆叠

	for (int Index = 9; Index < Items.Num(); ++Index)
	{
		FItemStack& ItemStack = Items[Index];
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

bool UBackpackComponent::AddItemStackBackpack(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	for (int Index = 9; Index < Items.Num(); ++Index)
	{
		FItemStack& ItemStack = Items[Index];
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

bool UBackpackComponent::AddItemToHotbar(FItemStack& ItemStack)
{
	if (ItemStack.IsEmpty()) return false;

	bool bSameFlag = AddSameItemToHotbar(ItemStack);

	bool bAddFlag = AddItemStackHotbar(ItemStack);

	if (bSameFlag || bAddFlag)
	{
		OnHotbarUpdate.Broadcast();
	}

	return ItemStack.IsEmpty();
}

bool UBackpackComponent::AddSameItemToHotbar(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	if (!InItemStack.IsStack()) return bFlag;		// 不可堆叠

	for (int Index = 0; Index < 8; ++Index)
	{
		FItemStack& ItemStack = Items[Index];
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

bool UBackpackComponent::AddItemStackHotbar(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	for (int Index = 0; Index < 8; ++Index)
	{
		FItemStack& ItemStack = Items[Index];
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

bool UBackpackComponent::IsHotbarIndex(int32 Index)
{
	return Index >= 0 && Index < 9;
}

void UBackpackComponent::NotifyAndUpdateUI(int32 Index)
{
	if (!IsValidIndex(Index)) return;

	if (IsHotbarIndex(Index))
	{
		OnHotbarUpdate.Broadcast();
	}
	else
	{
		OnInventoryUpdate.Broadcast();
	}
}

FItemStack UBackpackComponent::DecreStackSize(int32 Index, int32 Count)
{
	bool bFlag = false;

	FItemStack ReturnValue;

	if (IsValidIndex(Index))
	{
		FItemStack& ItemStack = Items[Index];
		if (!ItemStack.IsEmpty())
		{
			ReturnValue = ItemStack.SplitStack(Count);
			bFlag = true;
		}
	}

	if (bFlag)
	{
		NotifyAndUpdateUI(Index);
	}

	return ReturnValue;
}
