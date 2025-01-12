// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Utils/ItemStackHelper.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(InventorySize);
}

int32 UInventoryComponent::GetSizeInventory_Implementation() const
{
	return Items.Num();
}

bool UInventoryComponent::IsEmpty_Implementation() const
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

bool UInventoryComponent::IsEmptyFromIndex_Implementation(int32 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index].IsEmpty();
	}
	return false;
}

FItemStack UInventoryComponent::GetItemStack_Implementation(int32 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	return FItemStack();
}

FItemStack UInventoryComponent::DecrStackSize_Implementation(int32 Index, int32 Count)
{
	if (!Items.IsValidIndex(Index)) return FItemStack();

	FItemStack ItemStack = ItemStackHelper::GetAndSplit(Items, Index, Count);

	Notify(Index);

	return ItemStack;
}

FItemStack UInventoryComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	if (!Items.IsValidIndex(Index)) return FItemStack();

	FItemStack ItemStack = ItemStackHelper::GetAndRemove(Items, Index);

	Notify(Index);

	return ItemStack;
}

void UInventoryComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
	if (IsValidIndex(Index))
	{
		Items[Index] = Stack;

		Notify(Index);
	}
}

void UInventoryComponent::Clear_Implementation()
{
	for (FItemStack& ItemStack : Items)
	{
		ItemStack.Empty();
	}
}

void UInventoryComponent::Notify(int32 Index)
{
	OnInventoryItemUpdateDelegate.Broadcast(Index, GetItemStack_Implementation(Index));
}
