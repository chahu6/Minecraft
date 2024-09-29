// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemStack.h"
#include "Item/Items.h"
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

int32 FItemStack::GetStackSize() const
{
	return StackSize;
}

void FItemStack::SetStackSize(int32 NewStackSize)
{
	StackSize = NewStackSize;
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

bool FItemStack::operator==(const FItemStack& ItemStack) const
{
	return StackSize == ItemStack.StackSize && Item == ItemStack.Item;
}

bool FItemStack::operator!=(const FItemStack& ItemStack) const
{
	return !((*this) == ItemStack);
}
