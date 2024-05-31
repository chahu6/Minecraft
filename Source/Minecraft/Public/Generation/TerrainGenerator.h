#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TerrainGenerator.generated.h"

class AChunk;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UTerrainGenerator : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MINECRAFT_API ITerrainGenerator
{
	GENERATED_BODY()

public:
	virtual void GenerateChunk(AChunk* chunk) = 0;

	virtual void Populate(int32 X, int32 Y) = 0;
};
