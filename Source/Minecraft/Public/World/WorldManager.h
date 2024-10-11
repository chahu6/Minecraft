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
class FChunkData;
struct FItemStack;
class AEntityItem;
class ATileEntity;

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
	bool DestroyBlock(const FIntVector& BlockWorldVoxelLocation, bool bDropBlock = false);

	void PlaceBlock(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState);

	FBlockState GetBlockState(const FIntVector& BlockWorldVoxelLocation);

	TSharedPtr<FChunkData> GetChunkData(const FIntPoint& ChunkVoxelLocation);
	AChunk* GetChunk(const FIntPoint& ChunkVoxelLocation);

	FProgressDelegate ProgressDelegate;

	AEntityItem* SpawnEntity(const FVector& WorldLocation, const FItemStack& ItemStack);
	AEntityItem* SpawnEntity(const FIntVector& BlockWorldVoxelLocation, const FItemStack& ItemStack);

private:
	void SetBlockState(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState);

	void SetTileEntity(const FIntVector& BlockWorldVoxelLocation, ATileEntity* TileEntity);

	void InitialWorldChunkLoad();

	/*
	* 
	* 新版
	*/
	bool UpdatePosition();

	//void EnqueueDirtyChunk()

	void AddChunkToUpdate(const FIntVector& BlockWorldVoxelLocation, bool bTop = false);

	void AddChunkToUpdate(const FIntPoint& ChunkVoxelLocation, bool bTop = false);

	void ThreadUpdate();

	void UpdateChunks();

	void UpdateChunk();

	void RenderChunk();

	void UnloadChunk();

	void CheckSurroundingChunkNeedUpdate(const FIntVector& BlockOffsetLocation, int32 ChunkVoxelLocationX, int32 ChunkVoxelLocationY);

public:
	TQueue<FIntPoint, EQueueMode::Mpsc> SpawnChunkQueue;

	TQueue<FIntPoint, EQueueMode::Mpsc> DirtyChunkQueue;

	TQueue<FIntPoint, EQueueMode::Mpsc> UnloadChunkQueue;

	TMap<FIntPoint, AChunk*> ActiveChunks;

	UPROPERTY()
	TArray<class ATileEntity*> TileEntityList;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkManagerComponent> ChunkManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTerrainComponent> TerrainManager;

protected:
	UPROPERTY(EditAnywhere, Category = "World Setting")
	TSubclassOf<AEntityItem> DroppedItemClass;

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

	// 每次渲染Chunk的个数
	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 RenderCount = 2;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float UnloadRate = 0.02f;

	// 每次卸载Chunk的个数
	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 UnloadCount = 3;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float UpdateRate = 1.f;

	FTimerHandle RenderQueueHandle;

	FTimerHandle UpdateHandle;

	FTimerHandle UnloadHandle;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FIntPoint CharacterChunkPosition = { 1, 1 };

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FIntPoint PlayerPosition;

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
	TArray<FIntPoint> ChunksToUpdate;
	FCriticalSection ChunkUpdateCritical;

	FWorldRunner* ChunkUpdateThread;
	FChunkGenerateRunner* ChunkGenerateThread;

public:
	FORCEINLINE FVector2D GetDefaultCharacterPosition() const { return DefaultCharacterPosition; }
};
