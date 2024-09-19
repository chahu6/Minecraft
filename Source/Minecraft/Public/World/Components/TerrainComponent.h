// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TerrainComponent.generated.h"

class AChunk;
class AWorldManager;

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTerrainComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void LoadTerrainInfo(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos);

	void LoadTerrainBlockID(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos);

private:
	void GenerateHeight(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos);

	void GeneratePlant(AWorldManager* WorldManager, const FIntPoint& ChunkVoxelPos);

	float FBM(float InX, float InY, const TArray<FVector2D>& InOctaveOffsets, ETerrainFBMType InFBMType) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Terrain Setting")
	ETerrainFBMType BaseFBM = ETerrainFBMType::OrdinaryFBM;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting")
	int32 MaxBlockHeight = 128;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting")
	TObjectPtr<UCurveFloat> HeightRemap;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting", meta = (ClampMin = "0", ClampMax = "8"))
	int32 Octaves = 8;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting")
	FVector2D Offset = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting", Meta = (ClampMin = "0.0001"))
	float Scale = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting", Meta = (ClampMin = "0", ClampMax = "1"))
	float Persistance = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting", Meta = (ClampMin = "1"))
	float Lacunarity = 2.0f;

	/**
	* Domain Warping
	*/

	UPROPERTY(EditAnywhere, Category = "Terrain Setting|Domain Warping")
	bool bDomainWarping = false;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting|Domain Warping", Meta = (ClampMin = "0", EditCondition = "bDomainWarping"))
	ETerrainFBMType WarpingFBM = ETerrainFBMType::OrdinaryFBM;

	UPROPERTY(EditAnywhere, Category = "Terrain Setting|Domain Warping", Meta = (ClampMin = "0", EditCondition = "bDomainWarping"))
	float WarpingStrength = 40.0f;

	/**
	* Terrace
	*/ 
	UPROPERTY(EditAnywhere, Category = "Terrain Setting|Terrace")
	bool bTerrace = false;

private:
	TArray<float> NewNoiseMap;
	TArray<FVector2D> OctaveOffsets;
};
