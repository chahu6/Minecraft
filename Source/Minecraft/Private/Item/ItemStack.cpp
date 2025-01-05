// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemStack.h"
#include "Item/Item.h"
#include "MinecraftGameplayTags.h"

FItemStack::FItemStack()
{
	ItemID = AIR;
}

void FItemStack::Empty()
{
	ItemID = AIR;
	StackSize = 0;
}

bool FItemStack::IsEmpty() const
{
	return ItemID == AIR || StackSize == 0;
}

//bool FItemStack::IsFull() const
//{
//	return StackSize == Item->MaxStackSize;
//}

bool FItemStack::IsStack() const
{
	//return Item->bIsStack;
	return false;
}

int32 FItemStack::GetCount() const
{
	return StackSize;
}

void FItemStack::SetCount(int32 Size)
{
	StackSize = Size;

	if (StackSize <= 0) Empty();
}

UItem* FItemStack::GetItem() const
{
	return UItem::GetItemFromID(ItemID);
}

void FItemStack::SetItemID(const FGameplayTag& InItemID)
{
	ItemID = InItemID;
}

FGameplayTag FItemStack::GetItemID() const
{
	return ItemID;
}

int32 FItemStack::GetMaxStackSize() const
{
	return GetItem()->MaxStackSize;
}

FItemStack FItemStack::SplitStack(int32 Amount)
{
	const int32 Num = FMath::Min(Amount, StackSize);

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

bool FItemStack::operator==(const FItemStack& Other) const
{
	return ItemID == Other.ItemID && StackSize == Other.StackSize;
}

bool FItemStack::operator!=(const FItemStack& Other) const
{
	return !((*this) == Other);
}
