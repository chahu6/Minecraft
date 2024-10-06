// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemStack.h"
#include "Init/Items.h"
#include "Item/Item.h"

FItemStack::FItemStack()
	:StackSize(0)
{
	Item = const_cast<UItem*>(UItems::Air);
}

void FItemStack::Empty()
{
	*this = FItemStack();
}

bool FItemStack::IsEmpty() const
{
	return (*this) == FItemStack();
}

bool FItemStack::IsFull() const
{
	return StackSize == Item->MaxStackSize;
}

bool FItemStack::IsStack() const
{
	return Item->bIsStack;
}

int32 FItemStack::GetCount() const
{
	return StackSize;
}

void FItemStack::SetCount(int32 Size)
{
	StackSize = Size;

	if (StackSize <= 0)
	{
		Empty();
	}
}

const UItem* FItemStack::GetItem() const
{
	return Item;
}

void FItemStack::SetItem(UItem* NewItem)
{
	Item = NewItem;
}

int32 FItemStack::GetMaxStackSize() const
{
	return Item->MaxStackSize;
}

void FItemStack::Decrement()
{
	if (IsEmpty()) return;

	StackSize--;

	if (StackSize <= 0) Empty();
}

FItemStack FItemStack::SplitStack(int32 Amount)
{
	int32 Num = FMath::Min(Amount, StackSize);

	FItemStack ItemStack = *this;
	ItemStack.SetCount(Num);
	Shrink(Num);
	return ItemStack;
}

void FItemStack::Shrink(int32 Quantity)
{
	Grow(-Quantity);
}

void FItemStack::Grow(int32 Quantity)
{
	SetCount(StackSize + Quantity);
}

bool FItemStack::operator==(const FItemStack& ItemStack) const
{
	return StackSize == ItemStack.StackSize && Item == ItemStack.Item;
}

bool FItemStack::operator!=(const FItemStack& ItemStack) const
{
	return !((*this) == ItemStack);
}
