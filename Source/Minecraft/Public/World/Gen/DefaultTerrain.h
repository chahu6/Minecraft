// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Gen/TerrainBase.h"
#include "DefaultTerrain.generated.h"

class USimplexNoiseData;
class UBiome;
struct FGameplayTag;
class UBlock;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UDefaultTerrain : public UTerrainBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Generate_Implementation(AWorldManager* InWorldManager, const FChunkPos& InChunkPos) override;

	virtual void GenerateBiome_Implementation(AWorldManager* InWorldManager, const FChunkPos& InChunkPos) override;

protected:
	int32 GetRealHeightAndBiomes(float InX, float InY, FGameplayTag& OutBiomeTag, TTuple<float, float, float, float, float>& InNoises);

	const UBlock* GetBlock(int32 X, int32 Y, int32 Height, UBiome* Biome, int32 Z) const;

	//void SetBiome();

protected:
	/** ��½������� */
	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<USimplexNoiseData> Continent;

	/** ��ʴ���� */
	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<USimplexNoiseData> Erosion;

	/** ����������� */
	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<USimplexNoiseData> PeaksValleys;

	/** �������� */
	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<USimplexNoiseData> Temperature;

	/** ʪ������ */
	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<USimplexNoiseData> Humidity;

	/** ��Ѩ���� */
	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<USimplexNoiseData> Caves;

	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<UCurveFloat> ContinentalnessCurve;

	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<UCurveFloat> ErosionCurve;

	UPROPERTY(EditAnywhere, Category = "Terrain")
	TObjectPtr<UCurveFloat> PeakValleysCurve;

	UPROPERTY()
	TArray<UBiome*> BiomeMap;

	UPROPERTY(EditAnywhere, Category = "Terrain", Meta = (ClampMin = "0.0001"))
	float Scale = 100.0f;
};
