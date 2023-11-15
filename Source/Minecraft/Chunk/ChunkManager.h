#pragma once

#include "CoreMinimal.h"
#include "Minecraft/MinecraftType/BlockType.h"
#include "Minecraft/Generation/TerrainGenerator.h"

class AWorldManager;

class FChunkManager
{
	friend class AWorldManager;
public:
	FChunkManager() = default;
	FChunkManager(AWorldManager* WorldManager);

	void LoadChunk(int32 X, int32 Y);
	void UnLoadChunk(int32 X, int32 Y);

	AChunk* GetChunk(int32 X, int32 Y);

	EBlockType GetBlockType(int32 X, int32 Y, int32 Z);

private:
	TMap<FVector2D, class AChunk*> AllChunks;
	TMap<FVector2D, class AChunk*> PreLoadChunks;
	TUniquePtr<ITerrainGenerator> TerrainGenerator;
	AWorldManager* WorldManager;
};