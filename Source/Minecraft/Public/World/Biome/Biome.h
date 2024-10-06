// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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

	static TMap<int32, const UBiome*> REGISTER_ID;
	static TMap<FName, const UBiome*> REGISTER_NAME;

	static void RegisterBiomes();

	static void RegisterBiome(const UBiome* Biome);

public:
	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Biome)
	FPrimaryAssetType BiomeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 BiomeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	FName Name;
};
