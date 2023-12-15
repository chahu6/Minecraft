#include "BackpackComponent.h"
#include "Minecraft/Item/DroppedItem.h"

UBackpackComponent::UBackpackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Items.SetNum(36);
	ArmorItems.SetNum(4);
}

void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBackpackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FItemStack UBackpackComponent::GetSelected(int32 SelectedIndex)
{
	if (SelectedIndex >= 0 && SelectedIndex < 9)
	{
		return Items[SelectedIndex];
	}

	return FItemStack();
}

bool UBackpackComponent::AddItemToInventory(const ADroppedItem* DroppedItem)
{
	bool bFlags = false;

	FItemStack ItemStack = DroppedItem->GetItemStack();

	for (auto Itr = Items.CreateIterator(); Itr; ++Itr)
	{
		if (Itr->ID == ItemStack.ID)
		{
			int32 Sum = Itr->Amount + ItemStack.Amount;
			if (Sum <= ItemStack.MaxCount)
			{
				Itr->Amount = Sum;

				NotifyAndUpdateUI(Itr.GetIndex());
				return true;
			}
			else
			{
				int32 Remain = Sum - ItemStack.MaxCount;
				ItemStack.Amount = Remain;
			}
		}
	}

	for (auto Itr = Items.CreateIterator(); Itr; ++Itr)
	{
		if (Itr->ID < 0)
		{
			bFlags = true;
			*Itr = ItemStack;

			NotifyAndUpdateUI(Itr.GetIndex());
			break;
		}
	}

	return bFlags;
}

bool UBackpackComponent::TransferSlots(int32 SourceIndex, UBackpackComponent* SourceInventory, int32 DestinationIndex)
{
	if (Items.IsValidIndex(DestinationIndex))
	{
		FItemStack DestItemStack = Items[DestinationIndex];

		const auto& SourceItems = SourceInventory->GetItems();
		if (SourceItems.IsValidIndex(SourceIndex))
		{
			FItemStack SourceItemStack =  SourceItems[SourceIndex];
			//if (SourceItemStack.ID == DestItemStack.ID)
			{

			}
			//else
			{
				Items[DestinationIndex] = SourceItemStack;
				SourceInventory->SetItemStack(DestinationIndex, DestItemStack);
			}

			NotifyAndUpdateUI(DestinationIndex);
			return true;
		}
	}

	return false;
}

bool UBackpackComponent::RemoveItemFromInventory(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		Items[Index].Empty();

		NotifyAndUpdateUI(Index);
		return true;
	}

	return false;
}

bool UBackpackComponent::IsHotbarSlot(int32 Index)
{
	return Index >= 0 && Index < 9;
}

void UBackpackComponent::NotifyAndUpdateUI(int32 Index)
{
	if (IsHotbarSlot(Index))
	{
		OnHotbarUpdate.Broadcast();
	}
	else
	{
		OnInventoryUpdate.Broadcast();
	}
}

bool UBackpackComponent::SetItemStack(int32 Index, const FItemStack& NewItemStack)
{
	if (Items.IsValidIndex(Index))
	{
		Items[Index] = NewItemStack;
		return true;
	}

	return false;
}

