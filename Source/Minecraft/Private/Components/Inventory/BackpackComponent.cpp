// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/BackpackComponent.h"

UBackpackComponent::UBackpackComponent()
{
	InventorySize = 36;
	ArmorItems.SetNum(4);
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
	if (IsHotbarSlot(SelectedIndex))
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

bool UBackpackComponent::IsHotbarSlot(int32 Index)
{
	return Index >= 0 && Index < 9;
}
