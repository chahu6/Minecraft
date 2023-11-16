#pragma once

#include "CoreMinimal.h"

class AChunk;

class ITerrainGenerator
{
public:
	virtual ~ITerrainGenerator() = default;

	virtual void GenerateChunk(AChunk* chunk) = 0;
	virtual void Populate(int32 X, int32 Y) = 0;
};