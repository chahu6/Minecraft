#pragma once

#include "CoreMinimal.h"

class GenLayer
{
public:
	GenLayer(int64 Seed);
	virtual ~GenLayer() = default;

public:
	void InitializeAllBiomeGenerators();

	virtual int32* GetInts(int32 AreaX, int32 AreaY, int32 AreaWidth, int32 AreaHeight) = 0;

	void InitChunkSeed(int64 X, int64 Y);
protected:
	int32 NextInt(int32 Seed);

protected:
	int64 BaseSeed;
	GenLayer* Parent;

private:
	int64 ChunkSeed;
	int64 WorldGenSeed;
};