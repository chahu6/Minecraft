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

FItemStack UBackpackComponent::GetSelected(int32 SelectedIndex)
{
	HotbarSelectedIndex = SelectedIndex;

	if (IsHotbarSlot(HotbarSelectedIndex))
	{
		return ItemsData[HotbarSelectedIndex];
	}

	return FItemStack();
}

void UBackpackComponent::AfterDataUpdate(int32 Index)
{
	NotifyAndUpdateUI(Index);
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

bool UBackpackComponent::IsHotbarSlot(int32 Index)
{
	return Index >= 0 && Index < 9;
}
