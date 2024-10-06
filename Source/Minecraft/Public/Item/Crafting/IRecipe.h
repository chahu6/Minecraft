#pragma once

class UCraftingComponent;
struct FItemStack;

class IRecipe
{
public:
	virtual ~IRecipe() = default;

	virtual bool Matches(UCraftingComponent* CraftingMatrix) = 0;
	virtual FItemStack GetCraftingResult() = 0;
};