#include "Item/ItemStack.h"

FItemStack::FItemStack()
	:ID(-1), Amount(1), Item(nullptr), MaxCount(64)
{}

FItemStack::FItemStack(int32 ID, TSharedPtr<FItem> NewItem, int32 Amount)
	:ID(ID), Amount(Amount), Item(NewItem)
{}

bool FItemStack::IsEmpty()
{
	return ID < 0;
}

void FItemStack::Clear()
{
	ID = -1;
	Item = nullptr;
	Amount = 1;
	MaxCount = 64;
}
