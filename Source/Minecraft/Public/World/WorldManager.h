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
class FWorldRunner;

DECLARE_DELEGATE_OneParam(FProgressDelegate, float);

UCLASS()
class MINECRAFT_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
	friend class UChunkManagerComponent;
	friend class FTerrainDataAsyncTask;
	friend class FWorldRunner;

	static AWorldManager* Instance;

public:	
	AWorldManager();

	virtual void Tick(float DeltaTime) override;

	static AWorldManager* Get() { return Instance; }

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Key��Chunk��Voxel World��λ�ã�û�г���ChunkSize��λ��
	AChunk* GetChunk(const FIntPoint& ChunkVoxelLocation);

	AChunk* GetChunk(const FIntVector& BlockWorldVoxelLocation);

	bool DestroyBlock(const FIntVector& BlockWorldVoxelLocation);

	void PlaceBlock(const FIntVector& BlockWorldVoxelLocation, int32 BlockID);

	FBlockData GetBlock(const FIntVector& BlockWorldVoxelLocation);

	FProgressDelegate ProgressDelegate;

private:
	void SetBlock(const FIntVector& BlockWorldVoxelLocation, int32 BlockID);

	void InitialWorldChunkLoad();

	bool UpdatePosition();

	void AddChunk();

	void SpawnChunk(const FIntPoint& ChunkPosition);

	void LoadChunkInfo(const FIntPoint& ChunkPosition);

	void RemoveChunk();

	void RenderChunksAsync();

	void RenderChunk();



	/*
	* 
	* �°�
	*/
	void LoadChunks();

	void LoadWorld(const FIntPoint& OffsetPosition);

	void AddChunkToUpdate(AChunk* Chunk, bool bTop = false);

	void ThreadedUpdate();

	void UpdateChunks();

public:
	// ��Ⱦ��������������
	TQueue<AChunk*, EQueueMode::Mpsc> TaskQueue;

	TQueue<AChunk*, EQueueMode::Mpsc> DirtyChunkQueue;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkManagerComponent> ChunkManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTerrainComponent> TerrainManager;

protected:
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

	// ÿ����ȾChunk�ĸ���
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

	FAsyncTask<FTerrainDataAsyncTask>* TerrainDataAsyncTask = nullptr;


	/*
	* ���µ�
	*/
private:
	TArray<AChunk*> ChunksToUpdate;
	FCriticalSection ChunkUpdateCritical;

	FWorldRunner* ChunkUpdateThread;

public:
	FORCEINLINE FVector2D GetDefaultCharacterPosition() const { return DefaultCharacterPosition; }
};
