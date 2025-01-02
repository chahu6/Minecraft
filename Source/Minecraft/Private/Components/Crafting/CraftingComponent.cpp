#include "Components/Crafting/CraftingComponent.h"
#include "Utils/ItemStackHelper.h"

#include "Item/Crafting/CraftingManager.h"
#include "Item/Crafting/IRecipe.h"

UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
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
		NotifyAndUpdate();
	}

	return ItemStack;
}

FItemStack UCraftingComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	FItemStack ItemStack = ItemStackHelper::GetAndRemove(StackList, Index);

	NotifyAndUpdate();
	return ItemStack;
}

void UCraftingComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
	if (StackList.IsValidIndex(Index))
	{
		StackList[Index] = Stack;
		NotifyAndUpdate();
	}
}

bool UCraftingComponent::AddItemToInventoryFromIndex_Implementation(int32 Index, FItemStack& InItemStack)
{
	/*if (InItemStack.IsEmpty() || !StackList.IsValidIndex(Index)) return false;

	FItemStack& ItemStack = StackList[Index];

	if (ItemStack.IsEmpty())
	{
		ItemStack = InItemStack;
		InItemStack.Empty();
	}
	else
	{
		if (InItemStack.IsStack())
		{
			if (ItemStack.GetItem() == InItemStack.GetItem())
			{
				if (!ItemStack.IsFull())
				{
					int32 Sum = ItemStack.GetCount() + InItemStack.GetCount();
					if (Sum <= ItemStack.GetMaxStackSize())
					{
						ItemStack.SetCount(Sum);
						InItemStack.Empty();
					}
					else
					{
						ItemStack.SetCount(ItemStack.GetMaxStackSize());
						InItemStack.SetCount(Sum - ItemStack.GetCount());
					}
				}
			}
			else
			{
				FItemStack TempItemStack = ItemStack;
				ItemStack = InItemStack;
				InItemStack = TempItemStack;
			}
		}
		else
		{
			FItemStack TempItemStack = ItemStack;
			ItemStack = InItemStack;
			InItemStack = TempItemStack;
		}
	}

	NotifyAndUpdate();

	return InItemStack.IsEmpty();*/
	return false;
}

void UCraftingComponent::Clear_Implementation()
{
	for (FItemStack& ItemStack : StackList)
	{
		ItemStack.Empty();
	}
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

	NotifyAndUpdate();
}

void UCraftingComponent::Init()
{
	StackList.Init(FItemStack(), InventoryWidth * InventoryHeight);
}

void UCraftingComponent::NotifyAndUpdate()
{
	//OnCraftMatrixChanged();
	OnCraftingItem.Broadcast();
}

void UCraftingComponent::OnCraftMatrixChanged()
{
	FItemStack ItemStack;
	TSharedPtr<IRecipe> Recipe = CraftingManager::FindMatchingRecipe(this);
	if (Recipe.IsValid())
	{
		ItemStack = Recipe->GetCraftingResult();
	}

	StackList.Last() = ItemStack;
	//OutputItemStack = ItemStack;
}