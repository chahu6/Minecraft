#include "Utils/ItemStackHelper.h"

FItemStack ItemStackHelper::GetAndSplit(TArray<FItemStack>& Stacks, int32 Index, int32 Amount)
{
	if (Stacks.IsValidIndex(Index) && !Stacks[Index].IsEmpty() && Amount > 0)
	{
		return Stacks[Index].SplitStack(Amount);
	}
	return FItemStack();
}

FItemStack ItemStackHelper::GetAndRemove(TArray<FItemStack>& Stacks, int32 Index)
{
	if (Stacks.IsValidIndex(Index))
	{
		FItemStack ItemStack = Stacks[Index];
		Stacks[Index].Empty();
		return ItemStack;
	}
	return FItemStack();
}
