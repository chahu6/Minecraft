// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Crafting/IRecipe.h"


/**
 * 
 */
class ShapelessRecipes : public IRecipe
{
public:
	static TSharedPtr<IRecipe> Deserialize(TSharedPtr<FJsonObject> JsonRoot);
};
