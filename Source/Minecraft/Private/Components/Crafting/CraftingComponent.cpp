#include "Components/Crafting/CraftingComponent.h"

UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Items.SetNum(Dimension * Dimension);

	//Recipes.Emplace()
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCraftingComponent::CreateItem(int32 Index, const FItemStack& ItemStack)
{
	if (Items.IsValidIndex(Index))
	{
		Items[Index] = ItemStack;
	}

	OnCraftingItemStart.Broadcast();
}

bool UCraftingComponent::TransferSlot(int32 Index, const FItemStack& HangItemStack, FItemStack& NewItemStack)
{
	if (IsValidIndex(Index))
	{
		if (HangItemStack.ID == Items[Index].ID)
		{

		}
		else
		{
			FItemStack ItemStack_Temp;
			ItemStack_Temp = Items[Index];
			Items[Index] = HangItemStack;
			NewItemStack = ItemStack_Temp;
		}

		OnCraftingItemStart.Broadcast();
		return true;
	}

	return false;
}

void UCraftingComponent::CreateItemOutput(const FItemStack& ItemStack)
{
	OnCraftingItemCompleted.Broadcast();
}

bool UCraftingComponent::IsValidIndex(int32 Index)
{
	return Items.IsValidIndex(Index);
}

FItemStack UCraftingComponent::GetItem(int32 Index)
{
	if (IsValidIndex(Index))
	{
		return Items[Index];
	}

	return FItemStack();
}

void UCraftingComponent::SetItem(const FItemStack& ItemStack, int32 Index)
{
	Items[Index] = ItemStack;
}

void UCraftingComponent::IncreaseItemAmount(int32 Index)
{

}

void UCraftingComponent::DecreaseItemAmount(int32 Index)
{

}

void UCraftingComponent::RemoveItem(int32 Index)
{
	if (IsValidIndex(Index))
	{
		Items[Index].Clear();
	}
}

bool UCraftingComponent::TryAddItem(const FItemStack& ItemStack, int32 Index)
{
	if (IsValidIndex(Index))
	{
		SetItem(ItemStack, Index);
		return true;
	}
	else
	{
		if (ItemStack.ID == GetItem(Index).ID)
		{
			IncreaseItemAmount(Index);
			return true;
		}
	}

	return false;
}
