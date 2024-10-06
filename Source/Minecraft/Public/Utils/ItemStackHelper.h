#pragma once

#include "Item/ItemStack.h"

class ItemStackHelper
{
public:
	static FItemStack GetAndSplit(TArray<FItemStack>& Stacks, int32 Index, int32 Amount);

	static FItemStack GetAndRemove(TArray<FItemStack>& Stacks, int32 Index);
};