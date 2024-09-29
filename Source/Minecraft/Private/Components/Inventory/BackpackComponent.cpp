// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/BackpackComponent.h"

UBackpackComponent::UBackpackComponent()
{
	InventorySize = 36;

	ArmorItems.SetNum(4);

	ArmorItems_Test.SetNum(4);
}

void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBackpackComponent::NotifyAndUpdateUI()
{
	OnHotbarUpdate.Broadcast();
	OnInventoryUpdate.Broadcast();
}

FItemData UBackpackComponent::GetSelected(int32 SelectedIndex)
{
	if (IsHotbarIndex(SelectedIndex))
	{
		return Items[SelectedIndex];
	}

	return {};
}

void UBackpackComponent::ConsumeItem(int32 SelectedIndex)
{
	if (Items.IsValidIndex(SelectedIndex))
	{
		FItemData& ItemData = Items[SelectedIndex];
		if (ItemData.Quantity > 0)
		{
			ItemData.Quantity--;
		}
		if (ItemData.Quantity <= 0)
		{
			ItemData.Clear();
		}
		AfterDataUpdate();
	}
}

bool UBackpackComponent::AddItemToInventory(FItemData& ItemData)
{
	return Super::AddItemToInventory(ItemData);
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

	if (IsHotbarIndex(Index))
	{
		OnHotbarUpdate.Broadcast();
	}
	else
	{
		OnInventoryUpdate.Broadcast();
	}

	return bFlag;
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

	for (int Index = 9; Index < Items_Test.Num(); ++Index)
	{
		FItemStack& ItemStack = Items_Test[Index];
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

bool UBackpackComponent::AddItemStackBackpack(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	for (int Index = 9; Index < Items_Test.Num(); ++Index)
	{
		FItemStack& ItemStack = Items_Test[Index];
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
		FItemStack& ItemStack = Items_Test[Index];
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

bool UBackpackComponent::AddItemStackHotbar(FItemStack& InItemStack)
{
	bool bFlag = false;

	if (InItemStack.IsEmpty()) return bFlag;

	for (int Index = 0; Index < 8; ++Index)
	{
		FItemStack& ItemStack = Items_Test[Index];
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