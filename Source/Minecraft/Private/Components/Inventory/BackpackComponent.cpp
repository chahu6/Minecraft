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
	ItemStack.SetItemID(FMinecraftGameplayTags::Get().Workbench);
	Items[0] = ItemStack;

	FItemStack WoodItemStack;
	WoodItemStack.SetCount(64);
	WoodItemStack.SetItemID(FMinecraftGameplayTags::Get().Log);
	Items[1] = WoodItemStack;

	FItemStack ChestItemStack;
	ChestItemStack.SetCount(1);
	ChestItemStack.SetItemID(FMinecraftGameplayTags::Get().Chest);
	Items[2] = ChestItemStack;

	OnHotbarItemUpdateDelegate.Broadcast(0, Items[0]);
	OnHotbarItemUpdateDelegate.Broadcast(1, Items[1]);
	OnHotbarItemUpdateDelegate.Broadcast(2, Items[2]);
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
	if (!Items.IsValidIndex(Index)) return FItemStack();

	FItemStack ItemStack = ItemStackHelper::GetAndSplit(Items, Index, Count);

	Notify(Index);

	return ItemStack;
}

FItemStack UBackpackComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	if (!Items.IsValidIndex(Index)) return FItemStack();

	FItemStack ItemStack = ItemStackHelper::GetAndRemove(Items, Index);

	Notify(Index);

	return ItemStack;
}

void UBackpackComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
	if (IsValidIndex(Index))
	{
		Items[Index] = Stack;

		Notify(Index);
	}
}

void UBackpackComponent::Clear_Implementation()
{
	for (FItemStack& ItemStack : Items)
	{
		ItemStack.Empty();
	}
}

void UBackpackComponent::SetHangItemStack(const FItemStack& NewItemStack)
{
	HangItemStack = NewItemStack;
	OnHangItemUpdateDelegate.Broadcast(HangItemStack);
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

	DecrStackSize_Implementation(SelectedIndex, 1);
}

void UBackpackComponent::Notify(int32 Index)
{
	if (IsHotbarIndex(Index))
	{
		OnHotbarItemUpdateDelegate.Broadcast(Index, GetItemStack_Implementation(Index));
	}
	else
	{
		OnInventoryItemUpdateDelegate.Broadcast(Index, GetItemStack_Implementation(Index));
	}
}

bool UBackpackComponent::IsHotbarIndex(int32 Index)
{
	return Index >= 0 && Index < 9;
}
