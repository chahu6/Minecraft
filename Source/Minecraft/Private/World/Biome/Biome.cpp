// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Biome/Biome.h"
#include "MinecraftAssetManager.h"
#include "World/Biome/BiomeDecorator.h"
#include "Math/BlockPos.h"
#include "World/Gen/WorldGenTrees.h"
#include "World/Gen/WorldGenBigTree.h"

TMap<FGameplayTag, UBiome*> UBiome::REGISTER_NAME;

UBiome::UBiome()
{
	PrimaryAssetType = UMinecraftAssetManager::BiomeType;
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

	UE_LOG(LogTemp, Warning, TEXT("Biome REGISTER_NAME Num: %d"), REGISTER_NAME.Num());
}

void UBiome::RegisterBiome(UBiome* Biome)
{
	REGISTER_NAME.Add(Biome->BiomeID, Biome);
}

UBiome* UBiome::GetBiome(const FGameplayTag& InBiomeID)
{
	if (REGISTER_NAME.Contains(InBiomeID))
	{
		return REGISTER_NAME[InBiomeID];
	}
	check(false);
	return nullptr;
}

void UBiome::Decorate(AWorldManager* InWorldManager, const FBlockPos& Pos)
{
	check(Decorator);

	Decorator->Decorate(InWorldManager, this, Pos);
}

UWorldGenAbstractTree* UBiome::GetRandomTreeFeature()
{
	if (FMath::RandHelper(10) == 0)
	{
		return BIG_TREE_FEATURE;
	}
	else
	{
		return TREE_FEATURE;
	}
}

UWorldGenTallGrass* UBiome::GetRandomWorldGenForGrass()
{
	return TALLGRASS_FEATURE;
}

FString UBiome::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UBiome::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(PrimaryAssetType, GetFName());

	/*UPackage* Package = GetOutermost();

	if (!Package->HasAnyPackageFlags(PKG_PlayInEditor))
	{
		return FPrimaryAssetId(BiomeType, Package->GetFName());
	}

	return FPrimaryAssetId();*/
}
