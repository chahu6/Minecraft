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

int32 UInventoryComponent::GetSizeInventory_Implementation() const
{
	return Items.Num();
}

bool UInventoryComponent::IsEmpty_Implementation() const
{
	return false;
}

bool UInventoryComponent::IsEmptyFromIndex_Implementation(int32 Index) const
{
	return false;
}

FItemStack UInventoryComponent::GetItemStack_Implementation(int32 Index) const
{
	return FItemStack();
}

FItemStack UInventoryComponent::DecrStackSize_Implementation(int32 Index, int32 Count)
{
	return FItemStack();
}

FItemStack UInventoryComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	return FItemStack();
}

void UInventoryComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
}

bool UInventoryComponent::AddItemToInventoryFromIndex_Implementation(int32 Index, FItemStack& InItemStack)
{
	return false;
}

void UInventoryComponent::Clear_Implementation()
{
	for (FItemStack& ItemStack : Items)
	{
		ItemStack.Empty();
	}
}
