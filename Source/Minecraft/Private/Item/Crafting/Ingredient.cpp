#include "Item/Crafting/Ingredient.h"

Ingredient Ingredient::EMPTY;

Ingredient::Ingredient()
{
}

Ingredient::Ingredient(const TArray<FItemStack>& Stacks)
	:MatchingStacks(Stacks)
{
}

Ingredient Ingredient::FromStacks(const TArray<FItemStack>& Stacks)
{
	if (Stacks.Num() > 0)
	{
		return Ingredient(Stacks);
	}
	return Ingredient::EMPTY;
}

bool Ingredient::CheckMatch(const FItemStack& InItemStack)
{
	if (MatchingStacks.IsEmpty()) return InItemStack.IsEmpty();

	if (InItemStack.IsEmpty()) return false;

	for (const FItemStack& ItemStack : MatchingStacks)
	{
		if (ItemStack.GetItem() == InItemStack.GetItem())
		{
			return true;
		}
	}
	return false;
}
