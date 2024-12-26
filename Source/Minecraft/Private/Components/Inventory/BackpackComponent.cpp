// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/BackpackComponent.h"
#include "Item/Item.h"
#include "Init/Blocks.h"
#include "Utils/ItemStackHelper.h"

#include "MinecraftGameplayTags.h"

UBackpackComponent::UBackpackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(InventorySize);

	FItemStack ItemStack;
	ItemStack.SetCount(1);
	ItemStack.SetItem(UItem::GetItemFromBlock(UBlocks::CraftingTable));
	Items[0] = ItemStack;

	FItemStack WoodItemStack;
	WoodItemStack.SetCount(1);
	WoodItemStack.SetItem(UItem::GetItemFromBlock(UBlocks::Log));
	Items[1] = WoodItemStack;

	FItemStack ChestItemStack;
	ChestItemStack.SetCount(1);
	ChestItemStack.SetItem(UItem::GetItemFromID(FMinecraftGameplayTags::Get().Item_Block_Chest));
	Items[2] = ChestItemStack;
}

int32 UBackpackComponent::GetSizeInventory_Implementation() const
{
	return Items.Num();
}

bool UBackpackComponent::IsEmpty_Implementation() const
{
	for (const FItemStack& ItemStack : Items)
	{
		if (!ItemStack.IsEmpty())
		{
			return false;
		}
	}
	return true;
}

bool UBackpackComponent::IsEmptyFromIndex_Implementation(int32 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index].IsEmpty();
	}
	return false;
}

FItemStack UBackpackComponent::GetItemStack_Implementation(int32 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	return FItemStack();
}

FItemStack UBackpackComponent::DecrStackSize_Implementation(int32 Index, int32 Count)
{
	FItemStack ItemStack = ItemStackHelper::GetAndSplit(Items, Index, Count);

	if (!ItemStack.IsEmpty())
	{
		NotifyAndUpdateUI(Index);
	}

	return ItemStack;
}

FItemStack UBackpackComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	FItemStack ItemStack = ItemStackHelper::GetAndRemove(Items, Index);

	NotifyAndUpdateUI(Index);

	return ItemStack;
}

void UBackpackComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
	if (IsValidIndex(Index))
	{
		Items[Index] = Stack;
		NotifyAndUpdateUI(Index);
	}
}

void UBackpackComponent::Clear_Implementation()
{
	for (FItemStack& ItemStack : Items)
	{
		ItemStack.Empty();
	}
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

	if (IsValidIndex(SelectedIndex))
	{
		FItemStack& ItemStack = Items[SelectedIndex];
		ItemStack.Shrink(1);
	}

	NotifyAndUpdateUI(SelectedIndex);
}

bool UBackpackComponent::AddItemToInventory(FItemStack& ItemStack)
{
	bool bHotbar = AddItemToHotbar(ItemStack);
	bool bBackpack = AddItemToBackpack(ItemStack);
	return bHotbar || bBackpack;
}

void UBackpackComponent::PlaceItemBackInInventory(const FItemStack& ItemStack)
{

}

bool UBackpackComponent::AddItemToInventoryFromIndex_Implementation(int32 Index, FItemStack& InItemStack)
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

	NotifyAndUpdateUI(Index);

	return InItemStack.IsEmpty();
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

	if (!InItemStack.IsStack()) return bFlag;	// ���ɶѵ�

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

	if (!InItemStack.IsStack()) return bFlag;		// ���ɶѵ�

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
