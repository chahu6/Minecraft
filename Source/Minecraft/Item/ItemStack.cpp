#include "ItemStack.h"

FItemStack::FItemStack(int32 ID, TSharedPtr<FItem> NewItem, int32 Amount)
	:ID(ID), Item(NewItem), Amount(Amount)
{
}

void FItemStack::Empty()
{
	ID = -1;
	Item = nullptr;
	Amount = 1;
	MaxCount = 64;
}
