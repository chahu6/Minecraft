// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItemToInventory(FItemStack& ItemStack)
{
	bool bFlags = false;
	if (ItemStack.IsStack())
	{
		for (auto Itr = ItemsData.CreateIterator(); Itr; ++Itr)
		{
			if (Itr->ID == ItemStack.ID)
			{
				int32 Sum = Itr->Quantity + ItemStack.Quantity;
				if (Sum <= ItemStack.GetMaxCount())
				{
					Itr->Quantity = Sum;
					ItemStack.Clear();
					bFlags = true;
					AfterDataUpdate(Itr.GetIndex());
					break;
				}
				else
				{
					Itr->Quantity = Itr->GetMaxCount();
					int32 Remain = Sum - ItemStack.GetMaxCount();
					ItemStack.Quantity = Remain;
				}
				AfterDataUpdate(Itr.GetIndex());
			}
		}
	}
	
	if (ItemStack.Quantity > 0)
	{
		for (auto Itr = ItemsData.CreateIterator(); Itr; ++Itr)
		{
			if (Itr->IsEmpty())
			{
				bFlags = true;
				*Itr = ItemStack;
				ItemStack.Clear();
				AfterDataUpdate(Itr.GetIndex());
				break;
			}
		}
	}

	return bFlags;
}

bool UInventoryComponent::TransferSlots(FItemStack HangItemStack, FItemStack& NewHangItemStack, UInventoryComponent* DestinationInventory, int32 DestinationIndex)
{
	if (DestinationInventory->IsValidIndex(DestinationIndex))
	{
		FItemStack DestItemStack = DestinationInventory->GetItemStack(DestinationIndex);
		FItemStack ItemStack_Temp;

		if (DestItemStack.ID == HangItemStack.ID)
		{
			int32 Sum = DestItemStack.Quantity + HangItemStack.Quantity;
			if (Sum <= DestItemStack.GetMaxCount())
			{
				ItemStack_Temp.Quantity = Sum;
			}
			else
			{
				ItemStack_Temp.Quantity = DestItemStack.GetMaxCount();
				int32 Remain = Sum - DestItemStack.GetMaxCount();
				HangItemStack.Quantity = Remain;
			}
		}
		else
		{
			ItemStack_Temp = HangItemStack;
			HangItemStack = DestItemStack;
		}

		DestinationInventory->SetItemStack(DestinationIndex, ItemStack_Temp);
		NewHangItemStack = HangItemStack;
		//AfterDataUpdate(DestinationIndex);
		return true;
	}

	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(int32 Index)
{
	if (ItemsData.IsValidIndex(Index))
	{
		ItemsData[Index].Clear();

		//AfterDataUpdate(Index);
		return true;
	}

	return false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemsData.SetNum(InventorySize);
}

bool UInventoryComponent::IsValidIndex(int32 Index) const
{
	return ItemsData.IsValidIndex(Index);
}

void UInventoryComponent::AfterDataUpdate(int32 Index)
{
	NotifyAndUpdateUI(Index);
}

void UInventoryComponent::NotifyAndUpdateUI(int32 Index)
{
	OnInventoryUpdate.Broadcast();
}

FItemStack UInventoryComponent::GetItemStack(int32 Index) const
{
	if (IsValidIndex(Index))
	{
		return ItemsData[Index];
	}
	return FItemStack();
}

bool UInventoryComponent::SetItemStack(int32 Index, const FItemStack& NewItemStack)
{
	if (ItemsData.IsValidIndex(Index))
	{
		ItemsData[Index] = NewItemStack;
		return true;
	}

	return false;
}
