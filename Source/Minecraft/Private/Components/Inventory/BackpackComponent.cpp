// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/BackpackComponent.h"
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
	ItemStack.SetItemID(FMinecraftGameplayTags::Get().CraftingTable);
	Items[0] = ItemStack;

	FItemStack WoodItemStack;
	WoodItemStack.SetCount(64);
	WoodItemStack.SetItemID(FMinecraftGameplayTags::Get().Log);
	Items[1] = WoodItemStack;

	FItemStack ChestItemStack;
	ChestItemStack.SetCount(1);
	ChestItemStack.SetItemID(FMinecraftGameplayTags::Get().Chest);
	Items[2] = ChestItemStack;

	OnInventoryItemUpdateDelegate.Broadcast(0, Items[0]);
	OnInventoryItemUpdateDelegate.Broadcast(1, Items[1]);
	OnInventoryItemUpdateDelegate.Broadcast(2, Items[2]);
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
		OnInventoryItemUpdateDelegate.Broadcast(Index, GetItemStack_Implementation(Index));
	}

	return ItemStack;
}

FItemStack UBackpackComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	FItemStack ItemStack = ItemStackHelper::GetAndRemove(Items, Index);
	if (!ItemStack.IsEmpty())
	{
		OnInventoryItemUpdateDelegate.Broadcast(Index, GetItemStack_Implementation(Index));
	}
	return ItemStack;
}

void UBackpackComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
	if (IsValidIndex(Index))
	{
		Items[Index] = Stack;

		if (!Items[Index].IsEmpty())
		{
			OnInventoryItemUpdateDelegate.Broadcast(Index, Items[Index]);
		}
	}
}

void UBackpackComponent::Clear_Implementation()
{
	for (FItemStack& ItemStack : Items)
	{
		ItemStack.Empty();
	}
}

int32 UBackpackComponent::GetInventorySize() const
{
	return Items.Num();
}

FItemStack UBackpackComponent::GetItemStack(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	return FItemStack();
}

bool UBackpackComponent::IsEmpty(int32 Index) const
{
	if (IsValidIndex(Index))
	{
		return Items[Index].IsEmpty();
	}
	return false;
}

void UBackpackComponent::SetHangItemStack(const FItemStack& NewItemStack)
{
	HangItemStack = NewItemStack;
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
}

bool UBackpackComponent::IsHotbarIndex(int32 Index)
{
	return Index >= 0 && Index < 9;
}
