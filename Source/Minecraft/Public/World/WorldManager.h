#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Data/GenerationMethod.h"
#include "World/Data/GlobalInfo.h"
#include "WorldManager.generated.h"

class AChunk;
class UTerrainComponent;
class UChunkManagerComponent;
class FChunkGenerateRunner;
class FWorldRunner;
class UDataTable;
class FChunkData;

DECLARE_DELEGATE_OneParam(FProgressDelegate, float);

UCLASS()
class MINECRAFT_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
	friend class UChunkManagerComponent;
	friend class UTerrainComponent;
	friend class FWorldRunner;
	friend class FChunkGenerateRunner;

	static AWorldManager* Instance;

public:	
	AWorldManager();

	virtual void Tick(float DeltaTime) override;

	static AWorldManager* Get() { return Instance; }

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	bool DestroyBlock(const FIntVector& BlockWorldVoxelLocation);

	void PlaceBlock(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState);

	FBlockState GetBlockState(const FIntVector& BlockWorldVoxelLocation);

	TSharedPtr<FChunkData> GetChunkData(const FIntPoint& ChunkVoxelLocation);

	FProgressDelegate ProgressDelegate;

private:
	void SetBlockState(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState);

	void InitialWorldChunkLoad();

	bool UpdatePosition();

	void RenderChunk();

	/*
	* 
	* 新版
	*/
	void AddChunkToUpdate(AChunk* Chunk, bool bTop = false);

	void ThreadedUpdate();

	void UpdateChunks();

	void UpdateChunk();

public:
	// 渲染网格体的任务队列
	TQueue<AChunk*, EQueueMode::Mpsc> TaskQueue;

	TQueue<FIntPoint, EQueueMode::Mpsc> SpawnChunkQueue;

	TQueue<FIntPoint, EQueueMode::Mpsc> RemoveChunkQueue;

	TQueue<AChunk*, EQueueMode::Mpsc> DirtyChunkQueue;

	TMap<FIntPoint, AChunk*> ActiveChunks;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkManagerComponent> ChunkManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTerrainComponent> TerrainManager;

protected:
	UPROPERTY(EditAnywhere, Category = "World Setting")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	TObjectPtr<UDataTable> BlockDataTable;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 LoadDistance = 4;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 ChunkRenderRange = 8;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 Seed = 0;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	EGenerationMethod ChunkGenerationMethod = EGenerationMethod::Greedy;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float RenderRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float UpdateRate = 1.f;

	// 每次渲染Chunk的个数
	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 RenderCount = 1;

	FTimerHandle RenderQueueHandle;

	FTimerHandle UpdateHandle;

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

	/*
	* 最新的
	*/
	GlobalInfo WorldInfo;

private:
	TArray<AChunk*> ChunksToUpdate;
	FCriticalSection ChunkUpdateCritical;

	FWorldRunner* ChunkUpdateThread;
	FChunkGenerateRunner* ChunkGenerateThread;

public:
	FORCEINLINE FVector2D GetDefaultCharacterPosition() const { return DefaultCharacterPosition; }
};
