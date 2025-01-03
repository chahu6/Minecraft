// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SlotCrafting.h"
#include "Interfaces/InventoryInterface.h"
#include "Components/Crafting/CraftingComponent.h"

bool USlotCrafting::IsItemValid_Implementation(const FItemStack& ItemStack) const
{
	return false;
}

void USlotCrafting::OnTake_Implementation()
{
	check(CraftMatrix);
	const int32 Num = CraftMatrix->GetSizeInventory_Implementation();
	for (int32 i = 0; i < Num; ++i)
	{
		FItemStack ItemStack = CraftMatrix->GetItemStack_Implementation(i);
		if (!ItemStack.IsEmpty())
		{
			CraftMatrix->DecrStackSize_Implementation(i, 1);
		}
	}
}
