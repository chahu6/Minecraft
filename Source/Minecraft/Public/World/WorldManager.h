#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block/BlockID.h"
#include "World/GenerationMethod.h"
#include "WorldManager.generated.h"

class AChunk;
class UTerrainComponent;
class UChunkManagerComponent;
struct FBlockData;

DECLARE_DELEGATE_OneParam(FProgressDelegate, float);

UCLASS()
class MINECRAFT_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
	friend class UChunkManagerComponent;

public:	
	AWorldManager();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Key是Chunk在Voxel World的位置，没有乘以ChunkSize的位置
	AChunk* GetChunk(const FIntPoint& ChunkVoxelLocation);

	bool DestroyBlock(const FIntVector& BlockWorldVoxelLocation);

	void SetBlock(const FIntVector& BlockWorldVoxelLocation, int32 BlockID);

	FBlockData GetBlock(const FIntVector& BlockWorldVoxelLocation);

	FProgressDelegate ProgressDelegate;

	void RenderChunks();

private:
	void InitialWorldChunkLoad();

	bool UpdatePosition();

	void AddChunk();

	void CreateChunk(const FIntPoint& ChunkPosition);

	void LoadChunkInfo(const FIntPoint& ChunkPosition);

	void RemoveChunk();

	void RenderChunksAsync();

	//void Rebuild_Adjacent_Chunks(const FBlockPos& BlockPos);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z);

	void RenderChunk();

public:
	// 渲染网格体的任务队列
	TQueue<AChunk*, EQueueMode::Mpsc> TaskQueue;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkManagerComponent> ChunkManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTerrainComponent> TerrainManager;

protected:
	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 ChunkRenderRange = 8;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 Seed = 0;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	EGenerationMethod ChunkGenerationMethod = EGenerationMethod::Greedy;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float RenderRate = 0.1f;

	// 每次渲染Chunk的个数
	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 RenderCount = 1;

	FTimerHandle RenderQueueHandle;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FIntPoint CharacterChunkPosition;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D DefaultCharacterPosition;

	/*
	* First Initializer
	*/
	int32 Total = 0;

	std::atomic<int32> Count = 0;

public:
	FORCEINLINE FVector2D GetDefaultCharacterPosition() const { return DefaultCharacterPosition; }
};
