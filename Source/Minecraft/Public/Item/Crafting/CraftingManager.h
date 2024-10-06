// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class IRecipe;
class UCraftingComponent;

/**
 * 
 */
class MINECRAFT_API CraftingManager
{
public:
	static TMap<FName, TSharedPtr<IRecipe>> REGISTER;

	static void Init();

	static void Register(const FName& RecipeName, const TSharedPtr<IRecipe>& Recipe);

	static TSharedPtr<IRecipe> FindMatchingRecipe(UCraftingComponent* CraftingMatrix);

private:
	static void ParseJsonRecipes();

	static TSharedPtr<IRecipe> ParseRecipeJson(const TSharedPtr<FJsonObject>& JsonObject);
};
