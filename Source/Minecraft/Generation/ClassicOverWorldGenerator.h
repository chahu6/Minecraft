#pragma once
#include "TerrainGenerator.h"

class FClassicOverWorldGenerator : public ITerrainGenerator
{
public:
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

	class AChunk* CurrentChunk = nullptr;
};