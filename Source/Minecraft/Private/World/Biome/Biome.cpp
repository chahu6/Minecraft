// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Biome/Biome.h"
#include "MinecraftAssetManager.h"

TMap<int32, const UBiome*> UBiome::REGISTER_ID;
TMap<FName, const UBiome*> UBiome::REGISTER_NAME;

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

void UBiome::RegisterBiome(const UBiome* Biome)
{
	REGISTER_ID.Add(Biome->BiomeID, Biome);
	REGISTER_NAME.Add(Biome->Name, Biome);
}

FPrimaryAssetId UBiome::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(BiomeType, GetFName());
}
