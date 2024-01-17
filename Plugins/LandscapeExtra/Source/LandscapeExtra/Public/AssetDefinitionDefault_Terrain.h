// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "../Assets/LandscapeExtraTerrain.h"
#include "AssetDefinitionDefault_Terrain.generated.h"

/**
 * 
 */
UCLASS()
class LANDSCAPEEXTRA_API UAssetDefinitionDefault_Terrain : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	// UAssetDefinition Begin
	virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_Terrain", "Terrain"); }
	virtual FLinearColor GetAssetColor() const override { return FLinearColor(FColor(192, 64, 64)); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return ULandscapeExtraTerrain::StaticClass(); }
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const auto Categories = { EAssetCategoryPaths::UI };
		return Categories;
	}
	virtual bool CanImport() const override { return true; }
};
