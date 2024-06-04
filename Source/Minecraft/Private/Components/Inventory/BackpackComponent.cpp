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

bool UBackpackComponent::IsHotbarSlot(int32 Index)
{
	return Index >= 0 && Index < 9;
}
