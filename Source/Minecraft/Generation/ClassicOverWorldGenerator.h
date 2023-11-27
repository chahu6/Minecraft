#pragma once
#include "TerrainGenerator.h"

class AChunk;

class FClassicOverWorldGenerator : public ITerrainGenerator
{
public:
	FClassicOverWorldGenerator();
	FClassicOverWorldGenerator(int32 NewSeed);

	virtual void GenerateChunk(AChunk* Chunk) override;
	virtual void Populate(int32 X, int32 Y) override;

private:
	void SetBlocksInChunk();

	void GenerateHeightMap();
	void GenerateBiomeMap();

private:
	int32 Seed;

	TArray<uint8> HeightMap;

	AChunk* CurrentChunk = nullptr;
	FVector ChunkLocation;
};