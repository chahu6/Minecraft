#include "ItemStack.h"

FItemStack::FItemStack(Item* item, int32 Amount)
{
	this->item = item;
	this->count = Amount;
}
