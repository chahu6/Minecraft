#include "Components/Crafting/CraftingComponent.h"
#include "Utils/ItemStackHelper.h"
#include "Item/Crafting/CraftingManager.h"
#include "Item/Crafting/IRecipe.h"
#include "Components/Crafting/CraftingResultComponent.h"

UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	CraftingResult = GetOwner()->GetComponentByClass<UCraftingResultComponent>();

	StackList.Init(FItemStack(), InventoryWidth * InventoryHeight);
}

int32 UCraftingComponent::GetSizeInventory_Implementation() const
{
	return StackList.Num();
}

bool UCraftingComponent::IsEmpty_Implementation() const
{
	for (const FItemStack& ItemStack : StackList)
	{
		if (!ItemStack.IsEmpty())
		{
			return false;
		}
	}
	return true;
}

bool UCraftingComponent::IsEmptyFromIndex_Implementation(int32 Index) const
{
	if (StackList.IsValidIndex(Index))
	{
		return StackList[Index].IsEmpty();
	}
	return false;
}

FItemStack UCraftingComponent::GetItemStack_Implementation(int32 Index) const
{
	if (StackList.IsValidIndex(Index))
	{
		return StackList[Index];
	}
	return FItemStack();
}

FItemStack UCraftingComponent::DecrStackSize_Implementation(int32 Index, int32 Count)
{
	FItemStack ItemStack = ItemStackHelper::GetAndSplit(StackList, Index, Count);
	
	if (!ItemStack.IsEmpty())
	{
		OnCraftMatrixChanged();
	}

	OnItemUpdateDelegate.Broadcast(Index, GetItemStack_Implementation(Index));

	return ItemStack;
}

FItemStack UCraftingComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	FItemStack ItemStack = ItemStackHelper::GetAndRemove(StackList, Index);

	OnCraftMatrixChanged();
	return ItemStack;
}

void UCraftingComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
	if (StackList.IsValidIndex(Index))
	{
		StackList[Index] = Stack;

		OnItemUpdateDelegate.Broadcast(Index, Stack);
		OnCraftMatrixChanged();
	}
}

void UCraftingComponent::Clear_Implementation()
{
	for (FItemStack& ItemStack : StackList)
	{
		ItemStack.Empty();
	}
	OnCraftMatrixChanged();
}

FItemStack UCraftingComponent::GetStackInRowAndColumn(int32 Row, int32 Column)
{
	if (Row >= 0 && Row < InventoryWidth && Column >= 0 && Column < InventoryHeight)
	{
		return GetItemStack_Implementation(Row + Column * InventoryWidth);
	}
	return FItemStack();
}

void UCraftingComponent::ShrinkAllItems()
{
	for (FItemStack& ItemStack : StackList)
	{
		if (ItemStack.IsEmpty()) continue;

		ItemStack.Shrink(1);
	}

	OnCraftMatrixChanged();
}

void UCraftingComponent::OnCraftMatrixChanged()
{
	TSharedPtr<IRecipe> Recipe = FCraftingManager::FindMatchingRecipe(this);
	if (Recipe.IsValid())
	{
		const FItemStack ItemStack = Recipe->GetCraftingResult();
		CraftingResult->SetInventorySlotContents_Implementation(0, ItemStack);
	}
	else
	{
		CraftingResult->SetInventorySlotContents_Implementation(0, FItemStack());
	}
}