#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block/BlockID.h"
#include "WorldManager.generated.h"

class AChunk;
class AChunkSection;
class UClassicOverWorldGenerator;
class UChunkManagerComponent;
struct FBlockPos;
struct FBlockData;

UCLASS()
class MINECRAFT_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldManager();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Key是Chunk在Voxel World的位置，没有乘以ChunkSize的位置
	AChunk* GetChunk(const FVector2D& ChunkVoxelPosition);

	AChunkSection* GetChunkSection(const FVector& ChunkVoxelPosition);

	AChunkSection* GetChunkSection(const FBlockPos& BlockPos);

	bool DestroyBlock(const FBlockPos& BlockPos);

	void SetBlock(const FBlockPos& BlockPos, EBlockID BlockID);

	void SetBlock(const FBlockPos& BlockPos, int32 BlockID);

	FBlockData GetBlock(const FBlockPos& BlockPos);

private:

	void InitialWorldChunkLoad();

	bool UpdatePosition();

	void AddChunk();

	void RemoveChunk();

	void RenderChunks();

	void Rebuild_Adjacent_Chunks(const FBlockPos& BlockPos);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z);

public:
	TQueue<AChunk*, EQueueMode::Mpsc> TaskQueue;

private:
	UPROPERTY()
	TObjectPtr<UClassicOverWorldGenerator> TerrainGeneratorss;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkManagerComponent> ChunkManager;

	UPROPERTY(EditAnywhere)
	int32 ChunkRenderingRange = 8;

	UPROPERTY(EditAnywhere)
	int32 Seed = 0;

	FVector2D CharacterPosition;

public:
	FORCEINLINE UClassicOverWorldGenerator* GetTerrainGenerator() const { return TerrainGeneratorss; }
};
