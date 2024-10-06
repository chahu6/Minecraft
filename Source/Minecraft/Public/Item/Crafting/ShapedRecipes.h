// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Crafting/IRecipe.h"
#include "Item/ItemStack.h"

struct Ingredient;

/**
 * 
 */
class ShapedRecipes : public IRecipe
{
public:
	ShapedRecipes(FStringView InGroup, int32 Width, int32 Height, const TArray<Ingredient>& InRecipeItems, const FItemStack& Recipe);

	static TSharedPtr<IRecipe> Deserialize(TSharedPtr<FJsonObject> JsonRoot);
	static FItemStack DeserializeItem(TSharedPtr<FJsonObject> JsonRoot, bool bUseCount = true);
	static TMap<TCHAR, Ingredient> DeserializeKey(TSharedPtr<FJsonObject> JsonRoot);
	static Ingredient DeserializeIngredient(TSharedPtr<FJsonObject> JsonRoot);
	static TArray<Ingredient> DeserializeIngredients(const TArray<TSharedPtr<FJsonValue>>& PatternsArray, const TMap<TCHAR, Ingredient>& IngredientMap, int32 Width, int32 Height);

	/** IRecipe */
	virtual bool Matches(UCraftingComponent* Inv) override;
	virtual FItemStack GetCraftingResult() override;
	/** end IRecipe */

private:
	bool CheckMatch(UCraftingComponent* Inv, int32 Width, int32 Height);

private:
	FString Group;

	/** How many horizontal slots this recipe is wide. */
	int32 RecipeWidth;

	/** How many vertical slots this recipe uses. */
	int32 RecipeHeight;

	TArray<Ingredient> RecipeItems;

	FItemStack RecipeOutput;
};