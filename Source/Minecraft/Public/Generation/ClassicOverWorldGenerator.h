#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TerrainGenerator.h"
#include "ClassicOverWorldGenerator.generated.h"

class AChunk;

UCLASS(Blueprintable, BlueprintType)
class MINECRAFT_API UClassicOverWorldGenerator : public UObject, public ITerrainGenerator
{
	GENERATED_BODY()

public:
	UClassicOverWorldGenerator();

	virtual void GenerateChunk(AChunk* Chunk) override;

	virtual void Populate(int32 X, int32 Y) override;

private:
	void SetBlocksInChunk();

	void GenerateHeightMap();

	void GenerateBiomeMap();

private:
	UPROPERTY()
	AChunk* CurrentChunk;

	//UPROPERTY(EditAnywhere)
	//UCurveFloat* HeightRemap;

	int32 Seed;

	TArray<uint8> HeightMap;

	FVector ChunkLocation;
};