#include "BackpackComponent.h"
#include "Minecraft/Item/DroppedItem.h"

UBackpackComponent::UBackpackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Items.SetNum(27);
	HotbarItems.SetNum(9);
	ArmorItems.SetNum(4);
}

void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

	// Temp
	//Items[0].ID = 0;
}

void UBackpackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FItemStack UBackpackComponent::GetSelected()
{
	if (HotbarItems.IsValidIndex(SelectedIndex))
	{
		return HotbarItems[SelectedIndex];
	}
	else
	{
		return FItemStack();
	}
}

bool UBackpackComponent::AddItemToInventory(const ADroppedItem* DroppedItem)
{
	bool bFlags = false;

	FItemStack ItemStack = DroppedItem->GetItemStack();

	for (const auto& Elem : HotbarItems)
	{
		if (Elem.ID < 0)
		{

		}
	}

	for (auto& Elem : Items)
	{
		if (Elem.ID == ItemStack.ID)
		{
			int32 Sum = Elem.Amount + ItemStack.Amount;
			if (Sum <= ItemStack.MaxCount)
			{
				Elem.Amount = Sum;
				OnInventoryUpdate.Broadcast();
				return true;
			}
			else
			{
				int32 Temp = Sum - ItemStack.MaxCount;
				ItemStack.Amount = Temp;
			}
		}
	}

	for (auto& Elem : Items)
	{
		if (Elem.ID < 0)
		{
			bFlags = true;
			Elem = ItemStack;
			OnInventoryUpdate.Broadcast();
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

		return true;
	}

	return false;
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

