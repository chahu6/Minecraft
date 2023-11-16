#pragma once

#include "CoreMinimal.h"
#include "Minecraft/MinecraftType/BlockType.h"
#include "Minecraft/Generation/TerrainGenerator.h"
#include "Chunk.generated.h"

class FChunkManager
{
	friend class AWorldManager;
public:
	FChunkManager() = default;

private:
	TMap<FVector2D, class AChunk*> AllChunks;
	TMap<FVector2D, class AChunk*> PreLoadChunks;
	TUniquePtr<ITerrainGenerator> TerrainGenerator;
};