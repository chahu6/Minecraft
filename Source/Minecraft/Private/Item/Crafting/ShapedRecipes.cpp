// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Crafting/ShapedRecipes.h"
#include "Item/Item.h"
#include "Item/Crafting/Ingredient.h"
#include "Components/Crafting/CraftingComponent.h"

ShapedRecipes::ShapedRecipes(FStringView InGroup, int32 Width, int32 Height, const TArray<Ingredient>& InRecipeItems, const FItemStack& Recipe)
	:Group(InGroup),
	RecipeWidth(Width),
	RecipeHeight(Height),
	RecipeItems(InRecipeItems),
	RecipeOutput(Recipe)
{
}

TSharedPtr<IRecipe> ShapedRecipes::Deserialize(TSharedPtr<FJsonObject> JsonRoot)
{
	TSharedPtr<FJsonObject> KeyObject = JsonRoot->GetObjectField(TEXT("key"));
	TSharedPtr<FJsonObject> ResultObject = JsonRoot->GetObjectField(TEXT("result"));

	FString Group = JsonRoot->HasField(TEXT("group")) ? JsonRoot->GetStringField(TEXT("group")) : TEXT("");

	int32 Width = 0;
	int32 Height = 0;
	TMap<TCHAR, Ingredient> IngredientMap;

	TArray<TSharedPtr<FJsonValue>> PatternsArray = JsonRoot->GetArrayField(TEXT("pattern"));
	if (!PatternsArray.IsEmpty())
	{
		FString Pattern = PatternsArray[0]->AsString();

		Width = Pattern.Len();
		Height = PatternsArray.Num();

		IngredientMap = DeserializeKey(KeyObject);
	}

	TArray<Ingredient> RecipeItems = DeserializeIngredients(PatternsArray, IngredientMap, Width, Height);
	FItemStack ItemStack = DeserializeItem(ResultObject);

	return TSharedPtr<IRecipe>(new ShapedRecipes(Group, Width, Height, RecipeItems, ItemStack));
}

FItemStack ShapedRecipes::DeserializeItem(TSharedPtr<FJsonObject> JsonRoot, bool bUseCount)
{
	FItemStack ItemStack;

	FString ItemName = JsonRoot->HasField(TEXT("item")) ? JsonRoot->GetStringField(TEXT("item")) : TEXT("");
	int32 Count = bUseCount && JsonRoot->HasField(TEXT("count")) ? JsonRoot->GetNumberField(TEXT("count")) : 1;

	ItemStack.SetItemID(FGameplayTag::RequestGameplayTag(FName(ItemName)));
	ItemStack.SetCount(Count);

	return ItemStack;
}

TMap<TCHAR, Ingredient> ShapedRecipes::DeserializeKey(TSharedPtr<FJsonObject> JsonRoot)
{
	TMap<TCHAR, Ingredient> IngredientMap;
	TMap<FString, TSharedPtr<FJsonValue>>& EntrySet = JsonRoot->Values;
	for (TPair<FString, TSharedPtr<FJsonValue>>& Entry : EntrySet)
	{
		if (Entry.Key.Len() != 1 || Entry.Key == TEXT(" "))
		{
			break;
		}

		IngredientMap.Add(Entry.Key[0], DeserializeIngredient(Entry.Value->AsObject()));
	}

	return IngredientMap;
}

Ingredient ShapedRecipes::DeserializeIngredient(TSharedPtr<FJsonObject> JsonRoot)
{
	if (JsonRoot.IsValid())
	{
		TArray<FItemStack> ItemStacks;

		FItemStack ItemStack = DeserializeItem(JsonRoot, false);

		ItemStacks.Add(ItemStack);
		return Ingredient::FromStacks(ItemStacks);
	}
	return Ingredient::EMPTY;
}

TArray<Ingredient> ShapedRecipes::DeserializeIngredients(const TArray<TSharedPtr<FJsonValue>>& PatternsArray, const TMap<TCHAR, Ingredient>& IngredientMap, int32 Width, int32 Height)
{
	TArray<Ingredient> Ingredients;
	Ingredients.Init(Ingredient::EMPTY, Width * Height);

	TSet<TCHAR> Set;
	IngredientMap.GetKeys(Set);
	Set.Remove(' ');

	for (int32 i = 0; i < PatternsArray.Num(); ++i)
	{
		FString Pattern = PatternsArray[i]->AsString();
		for (int32 j = 0; j < Pattern.Len(); ++j)
		{
			const Ingredient* Recipe = IngredientMap.Find(Pattern[j]);
			if (Recipe)
			{
				Set.Remove(Pattern[j]);
				Ingredients[j + Width * i] = *Recipe;
			}
		}
	}

	if (Set.IsEmpty())
	{
		return Ingredients;
	}

	return TArray<Ingredient>();
}

bool ShapedRecipes::Matches(UCraftingComponent* Inv)
{
	for (int32 i = 0; i <= 3 - RecipeWidth; ++i)
	{
		for (int32 j = 0; j <= 3 - RecipeHeight; ++j)
		{
			if (CheckMatch(Inv, i, j))
			{
				return true;
			}
		}
	}
	return false;
}

FItemStack ShapedRecipes::GetCraftingResult()
{
	return RecipeOutput;
}

bool ShapedRecipes::CheckMatch(UCraftingComponent* Inv, int32 Width, int32 Height)
{
	for (int32 i = 0; i < 3; ++i)
	{
		for (int32 j = 0; j < 3; ++j)
		{
			int32 k = i - Width;
			int32 l = j - Height;
			Ingredient ingredient;

			if (k >= 0 && l >= 0 && k < RecipeWidth && l < RecipeHeight)
			{
				ingredient = RecipeItems[k + l * RecipeWidth];
			}

			FItemStack ItemStack = Inv->GetStackInRowAndColumn(i, j);
			if (!ingredient.CheckMatch(ItemStack))
			{
				return false;
			}
		}
	}
	return true;
}
