// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Crafting/CraftingManager.h"
#include "Item/Crafting/IRecipe.h"
#include "Item/Crafting/ShapedRecipes.h"
#include "Item/Crafting/ShapelessRecipes.h"
#include "Components/Crafting/CraftingComponent.h"

TMap<FName, TSharedPtr<IRecipe>> FCraftingManager::REGISTER;

void FCraftingManager::Init()
{
	ParseJsonRecipes();
}

void FCraftingManager::Register(const FName& RecipeName, const TSharedPtr<IRecipe>& Recipe)
{
	REGISTER.Add(RecipeName, Recipe);
}

TSharedPtr<IRecipe> FCraftingManager::FindMatchingRecipe(UCraftingComponent* CraftingMatrix)
{
	for (TPair<FName, TSharedPtr<IRecipe>>& Pair : REGISTER)
	{
		if (Pair.Value->Matches(CraftingMatrix))
		{
			return Pair.Value;
		}
	}
	return TSharedPtr<IRecipe>();
}

void FCraftingManager::ParseJsonRecipes()
{
	TArray<FString> FilePath;
	FString FileType = TEXT("*.json");
	
	FString RecipePath = FPaths::ProjectDir() + TEXT("Content/Assets/Recipes/");
	IFileManager& MyFileManager = IFileManager::Get();
	if (MyFileManager.DirectoryExists(RecipePath.GetCharArray().GetData()))
	{
		TArray<FString> AssetNames;
		MyFileManager.FindFiles(AssetNames, RecipePath.GetCharArray().GetData(), TEXT("*.json"));
		
		for (FString AssetName : AssetNames)
		{
			FString JsonData;
			FFileHelper::LoadFileToString(JsonData, *(RecipePath + AssetName));

			TSharedPtr<FJsonObject> JsonRoot = MakeShareable(new FJsonObject);
			TSharedRef<FJsonStringReader> JsonReader = FJsonStringReader::Create(JsonData);

			if (FJsonSerializer::Deserialize<TCHAR>(JsonReader, JsonRoot))
			{
				TArray<FString> Array;
				AssetName.ParseIntoArray(Array, TEXT("."));
				Register(FName(Array[0]), ParseRecipeJson(JsonRoot));
			}
		}
	}
}

TSharedPtr<IRecipe> FCraftingManager::ParseRecipeJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	TSharedPtr<FJsonValue> TypeValue = JsonObject->TryGetField(TEXT("type"));
	if (TypeValue.IsValid())
	{
		FString TypeValueString = TypeValue->AsString();
		if (TypeValueString.Equals(TEXT("crafting_shaped")))
		{
			return ShapedRecipes::Deserialize(JsonObject);
		}
		else if (TypeValueString.Equals(TEXT("crafting_shapeless")))
		{
			return ShapelessRecipes::Deserialize(JsonObject);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Loading Recipe Failed!"));
		}
	}

	return nullptr;
}
