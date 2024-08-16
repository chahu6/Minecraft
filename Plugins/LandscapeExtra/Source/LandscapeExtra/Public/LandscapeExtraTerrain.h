#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/TerrainInfo.h"
#include "LandscapeExtraTerrain.generated.h"

DECLARE_DELEGATE(FOnAssetPropertyChanged);
DECLARE_DELEGATE_TwoParams(FOnTestChanged, float, float);

/**
 * 
 */
UCLASS()
class LANDSCAPEEXTRA_API ULandscapeExtraTerrain : public UObject
{
	GENERATED_BODY()

public:
	ULandscapeExtraTerrain();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	FOnAssetPropertyChanged OnAssetPropertyChanged;

	FOnTestChanged OnTestChanged;

public:
	UPROPERTY(EditAnywhere, Category = "Base Noise")
	ETerrainFBMType BaseFBM = ETerrainFBMType::OrdinaryFBM;

	UPROPERTY(EditAnywhere, Category = "Base Noise")
	FRuntimeFloatCurve HeightRemap;

	UPROPERTY(EditAnywhere, Category = "Base Noise", Meta = (ClampMin = "0", ClampMax = "255"))
	float Amplitude = 100.0f;

	UPROPERTY()
	FRichCurve ColorRemap[4];

	UPROPERTY(EditAnywhere, Category = "Base Noise", Meta = (ClampMin = "0.0001"))
	float Scale = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Base Noise", Meta = (ClampMin = "0", ClampMax = "8"))
	int32 Octaves = 3;

	UPROPERTY(EditAnywhere, Category = "Base Noise", Meta = (ClampMin = "0", ClampMax = "1"))
	float Persistance = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Base Noise", Meta = (ClampMin = "1"))
	float Lacunarity = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Base Noise")
	FVector2D Offset = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Domain Warping")
	bool bDomainWarping = false;

	UPROPERTY(EditAnywhere, Category = "Domain Warping", Meta = (ClampMin = "0", EditCondition = "bDomainWarping"))
	ETerrainFBMType WarpingFBM = ETerrainFBMType::OrdinaryFBM;

	UPROPERTY(EditAnywhere, Category = "Domain Warping", Meta = (ClampMin = "0", EditCondition = "bDomainWarping"))
	float WarpingStrength = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Terrace")
	bool bTerrace = false;

	UPROPERTY(EditAnywhere, Category = "Terrace", Meta = (ClampMin = "0", EditCondition = "bTerrace"))
	ETerraceEdgeType EdgeType = ETerraceEdgeType::Sharp;

	UPROPERTY(EditAnywhere, Category = "Terrace", Meta = (ClampMin = "0", EditCondition = "bTerrace"))
	FVector2D HeightRange = FVector2D(0.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Terrace", Meta = (ClampMin = "1", EditCondition = "bTerrace"))
	float Slope = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Terrace", Meta = (ClampMin = "0", ClampMax = "1", EditCondition = "bTerrace", AllowPreserveRatio))
	FVector2D StepSize = FVector2D(0.1f, 0.1f);

	UPROPERTY(EditAnywhere, Category = "Preview Settings", meta = (ClampMin = "2", ClampMax = "256"))
	FIntVector2 TerrainSize = { 100, 100 };

	UPROPERTY(EditAnywhere, Category = "Preview Settings")
	int32 Seed = 42;

	UPROPERTY()
	UTextureRenderTarget2D* NoiseMapRenderTarget;

	UPROPERTY(EditAnywhere, Category = "Canyons")
	bool bCanyons = false;

	UPROPERTY(EditAnywhere, Category = "Canyons", Meta = (EditCondition = "bCanyons"))
	float Power = 0.f;

public:
	UPROPERTY(Transient)
	TArray<float> Data_Terrain_Default;
};
