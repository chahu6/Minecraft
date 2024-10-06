#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStack.h"

struct Ingredient
{
	static Ingredient EMPTY;

	Ingredient();

	Ingredient(const TArray<FItemStack>& Stacks);

	static Ingredient FromStacks(const TArray<FItemStack>& Stacks);

	bool CheckMatch(const FItemStack& InItemStack);

private:
	TArray<FItemStack> MatchingStacks;
};
