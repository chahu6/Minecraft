// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "World/Biome/BiomeID.h"
#include "Biome.generated.h"

class UBlock;
class UBiomeDecorator;
class AWorldManager;
struct FBlockPos;

class UWorldGenAbstractTree;
class UWorldGenTrees;
class UWorldGenBigTree;

/**
 * 
 */
UCLASS(abstract, BlueprintType)
class MINECRAFT_API UBiome : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBiome();

	static TMap<EBiomeID, UBiome*> REGISTER_ID;
	static TMap<FName, UBiome*> REGISTER_NAME;

	static void RegisterBiomes();

	static void RegisterBiome(UBiome* Biome);

	static UBiome* GetBiome(const FGameplayTag& BiomeTag);
	static UBiome* GetBiome(EBiomeID BiomeID);

	virtual void Decorate(AWorldManager* InWorldManager, const FBlockPos& Pos);

	virtual UWorldGenAbstractTree* GetRandomTreeFeature();

public:
	UFUNCTION(BlueprintCallable)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPrimaryAssetType PrimaryAssetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	EBiomeID BiomeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	int32 ShallowSurfaceDepth = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UBlock* TopBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UBlock* FillerBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UBiomeDecorator* Decorator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	int32 TreesPerChunk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	float ExtraTreeChance = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	int32 GrassPerChunk = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UWorldGenTrees* TREE_FEATURE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UWorldGenBigTree* BIG_TREE_FEATURE;
};
