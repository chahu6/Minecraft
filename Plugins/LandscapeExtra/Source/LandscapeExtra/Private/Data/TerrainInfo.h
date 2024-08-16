#pragma once

#include "CoreMinimal.h"
#include "TerrainInfo.generated.h"

UENUM(BlueprintType)
enum class ETerrainFBMType : uint8
{
	OrdinaryFBM			UMETA(DisplayName = "Ordinary"),
	TurbulenceFBM		UMETA(DisplayName = "Turbulence"), // ÍÄÁ÷
	RidgeFBM			UMETA(DisplayName = "Ridge")	   // É½¼¹
};

UENUM(BlueprintType)
enum class ETerraceEdgeType : uint8
{
	Normal				UMETA(DisplayName = "Normal"),
	Sharp				UMETA(DisplayName = "Sharp"),
	Smooth				UMETA(DisplayName = "Smooth"),
};

struct FTerrainMeshInfo
{
	int32 Width;

	int32 Height;

	TArray<float> NoiseMap;
};

struct FTerrainNoiseInfo
{
	ETerrainFBMType BaseFBM;

	ETerrainFBMType WarpingFBM;

	ETerraceEdgeType EdgeType;

	float Slope;

	bool bTerrace;

	bool bCanyons;

	float Power;

	FVector2D StepSize;

	bool bDomainWarping;

	int32 Seed;

	int32 Octaves;

	int32 Width;

	int32 Height;

	float Amplitude;

	float Scale;

	float Lacunarity;

	float Persistance;

	float WarpingStrength;

	FVector2D Offset;

	FRichCurve* HeightRemap;

	UTextureRenderTarget2D* NoiseMapRenderTarget;
};