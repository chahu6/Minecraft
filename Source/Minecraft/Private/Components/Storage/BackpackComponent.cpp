#include "Components/Storage/BackpackComponent.h"
#include "Item/DroppedItem.h"
#include "Entity/Player/MCPlayer.h"

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
	HotbarSelectedIndex = SelectedIndex;

	if (IsHotbarSlot(HotbarSelectedIndex))
	{
		return Items[HotbarSelectedIndex];
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
			}
			else
			{
				Itr->Amount = Itr->MaxCount;
				int32 Remain = Sum - ItemStack.MaxCount;
				ItemStack.Amount = Remain;
			}

			AfterDataUpdate(Itr.GetIndex());
			return true;
		}
	}

	for (auto Itr = Items.CreateIterator(); Itr; ++Itr)
	{
		if (Itr->ID < 0)
		{
			bFlags = true;
			*Itr = ItemStack;

			AfterDataUpdate(Itr.GetIndex());
			break;
		}
	}

	return bFlags;
}

bool UBackpackComponent::TransferSlots(FItemStack HangItemStack, FItemStack& NewHangItemStack, UBackpackComponent* DestinationInventory, int32 DestinationIndex)
{
	if (DestinationInventory->GetItems().IsValidIndex(DestinationIndex))
	{
		FItemStack DestItemStack = DestinationInventory->GetItemStack(DestinationIndex);
		FItemStack ItemStack_Temp;

		if (DestItemStack.ID == HangItemStack.ID)
		{
			int32 Sum = DestItemStack.Amount + HangItemStack.Amount;
			if (Sum <= DestItemStack.MaxCount)
			{
				ItemStack_Temp.Amount = Sum;
			}
			else
			{
				ItemStack_Temp.Amount = DestItemStack.MaxCount;
				int32 Remain = Sum - DestItemStack.MaxCount;
				HangItemStack.Amount = Remain;
			}
		}
		else
		{
			ItemStack_Temp = HangItemStack;
			HangItemStack = DestItemStack;
		}

		DestinationInventory->SetItemStack(DestinationIndex, ItemStack_Temp);
		NewHangItemStack = HangItemStack;
		AfterDataUpdate(DestinationIndex);
		return true;
	}

	return false;
}

bool UBackpackComponent::RemoveItemFromInventory(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		Items[Index].Clear();

		AfterDataUpdate(Index);
		return true;
	}

	return false;
}

void UBackpackComponent::ConsumeItemStack()
{
	Items[HotbarSelectedIndex].Amount--;
	if (Items[HotbarSelectedIndex].Amount <= 0)
	{
		Items[HotbarSelectedIndex].Clear();
	}

	OnHotbarUpdate.Broadcast();
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

void UBackpackComponent::AfterDataUpdate(int32 Index)
{
	if (IsHotbarSlot(Index))
	{
		if (Player)
		{
			Player->UpdateMainHandItem();
		}
	}

	NotifyAndUpdateUI(Index);
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

