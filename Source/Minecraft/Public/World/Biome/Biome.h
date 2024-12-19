// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Biome.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MINECRAFT_API UBiome : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBiome();

	static TMap<int32, UBiome*> REGISTER_ID;
	static TMap<FName, UBiome*> REGISTER_NAME;

	static void RegisterBiomes();

	static void RegisterBiome(UBiome* Biome);

	static UBiome* GetBiome(const FGameplayTag& BiomeTag);

public:
	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Biome)
	FPrimaryAssetType BiomeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 BiomeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	FGameplayTag Tag;
};
