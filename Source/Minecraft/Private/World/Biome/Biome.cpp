// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Biome/Biome.h"
#include "MinecraftAssetManager.h"

TMap<int32, UBiome*> UBiome::REGISTER_ID;
TMap<FName, UBiome*> UBiome::REGISTER_NAME;

UBiome::UBiome()
{
	BiomeType = UMinecraftAssetManager::BiomeType;
}

void UBiome::RegisterBiomes()
{
	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<UObject*> ObjectList;
	AssetManager.GetPrimaryAssetObjectList(UMinecraftAssetManager::BiomeType, ObjectList);

	for (UObject* Object : ObjectList)
	{
		if (UBiome* Biome = Cast<UBiome>(Object))
		{
			RegisterBiome(Biome);
		}
	}
}

void UBiome::RegisterBiome(UBiome* Biome)
{
	REGISTER_ID.Add(Biome->BiomeID, Biome);
	REGISTER_NAME.Add(Biome->Tag.GetTagName(), Biome);
}

UBiome* UBiome::GetBiome(const FGameplayTag& BiomeTag)
{
	if (REGISTER_NAME.Contains(BiomeTag.GetTagName()))
	{
		return REGISTER_NAME[BiomeTag.GetTagName()];
	}
	check(false);
	return nullptr;
}

FString UBiome::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UBiome::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(BiomeType, GetFName());

	/*UPackage* Package = GetOutermost();

	if (!Package->HasAnyPackageFlags(PKG_PlayInEditor))
	{
		return FPrimaryAssetId(BiomeType, Package->GetFName());
	}

	return FPrimaryAssetId();*/
}
