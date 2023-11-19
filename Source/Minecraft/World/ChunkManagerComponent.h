#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Minecraft/MinecraftType/BlockType.h"
#include "Minecraft/Generation/TerrainGenerator.h"


#include "ChunkManagerComponent.generated.h"

class AWorldManager;
class AChunk;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINECRAFT_API UChunkManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AWorldManager;
public:
	UChunkManagerComponent();

	AChunk* GetChunk(const FVector& Key);

	void LoadChunk(const FVector& ChunkVoxelPosition);

private:
	void Rebuild_Adjacent_Chunks(const FVector& ChunkVoxelWorldPosition);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z);

private:
	TMap<FVector, AChunk*> _AllChunks;
	TUniquePtr<ITerrainGenerator> _TerrainGenerator;
};