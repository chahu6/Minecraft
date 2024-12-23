#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Data/GenerationMethod.h"
#include "World/Data/GlobalInfo.h"
#include "Math/ChunkPos.h"
#include "World/Biome/BiomeID.h"
#include "WorldManager.generated.h"

class AChunk;
class UChunkPoolComponent;
class FWorldRunner;
struct FChunkData;
struct FItemStack;
class AEntityItem;
class ATileEntity;
class FChunkTickRunner;
class UChunkTaskPoolComponent;
class UTerrainBase;
class UWorldProviderComponent;
struct FBlockPos;

DECLARE_DELEGATE_OneParam(FProgressDelegate, float);

UCLASS()
class MINECRAFT_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
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
	bool DestroyBlock(const FIntVector& BlockWorldVoxelLocation, bool bDropBlock = false);

	void PlaceBlock(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState);

	FBlockState GetBlockState(const FIntVector& BlockWorldVoxelLocation);
	FBlockState GetBlockState(const FBlockPos& InBlockPos);
	FBlockState GetBlockState(int32 X, int32 Y, int32 Z);

	TSharedPtr<FChunkData> GetChunkData(const FChunkPos& InChunkPos) const;
	TSharedPtr<FChunkData> GetChunkData(const FBlockPos& InBlockPos) const;
	TSharedPtr<FChunkData> GetChunkData(int32 X, int32 Y) const;
	AChunk* GetChunk(const FChunkPos& InChunkPos) const;

	FProgressDelegate ProgressDelegate;

	AEntityItem* SpawnEntity(const FVector& WorldLocation, const FItemStack& ItemStack);
	AEntityItem* SpawnEntity(const FIntVector& BlockWorldVoxelLocation, const FItemStack& ItemStack);

	FORCEINLINE uint64 GetCurrentTick() const { return m_Tick.load(); }
	FORCEINLINE void NextTick()
	{
		m_Tick.fetch_add(1);
	}
	FORCEINLINE FChunkPos GetCenterChunkPos() const { return CenterChunkPos; }

	void LoadChunks(const TArray<FChunkPos>& InChunksPos);
	void LoadChunk(const FChunkPos& InChunkPos);

	void UnloadChunks(const TSet<FChunkPos>& InChunksPos);
	void UnloadChunk(const FChunkPos& InChunkPos);

	AChunk* SpawnChunk(const FChunkPos& InChunkPos);

	FBlockPos GetHeight(const FBlockPos& Pos);
	int32 GetHeight(int32 X, int32 Y);

	EBiomeID GetBiome(const FBlockPos& Pos);

	void GetNoises(const FBlockPos& InBlockPos, TTuple<float, float, float, float, float>& OutNoises);

	bool SetBlockState(const FBlockPos& InBlockPos, const FBlockState& BlockState);
	bool SetBlockState(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState);
	bool SetBlockState(int32 X, int32 Y, int32 Z, const FBlockState& BlockState);

private:
	void SetTileEntity(const FIntVector& BlockWorldVoxelLocation, ATileEntity* TileEntity);

	void InitialWorldChunkLoad();

	/*
	* 
	* 新版
	*/
	void AddChunkToUpdate(const FIntVector& BlockWorldVoxelLocation, bool bTop = false);

	void AddChunkToUpdate(const FIntPoint& ChunkVoxelLocation, bool bTop = false);

	void ThreadUpdate();

	void UpdateChunks();

	void UpdateChunk();

	void RenderChunk();

	void UnloadChunk();

	void CheckSurroundingChunkNeedUpdate(const FIntVector& BlockOffsetLocation, int32 ChunkVoxelLocationX, int32 ChunkVoxelLocationY);



	bool CoordsChanged();
	void SetCenterPos(const FChunkPos& InChunkPos);

	void UpdateWorld();

	void RemoveChunk(const FChunkPos& InChunkPos);

public:
	TQueue<FChunkPos, EQueueMode::Mpsc> LoadChunkQueue;
	TQueue<FChunkPos, EQueueMode::Mpsc> DirtyChunkQueue;
	TQueue<FChunkPos, EQueueMode::Mpsc> UnloadChunkQueue;

	UPROPERTY()
	TArray<class ATileEntity*> TileEntityList;

	/*
	* 最新的
	*/
	GlobalInfo WorldInfo;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkPoolComponent> ChunkPool;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkTaskPoolComponent> ChunkTaskPool;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWorldProviderComponent> WorldProvider;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	TSubclassOf<UTerrainBase> TerrainBaseClass;

	UPROPERTY()
	TObjectPtr<UTerrainBase> TerrainBase;

protected:
	UPROPERTY(EditAnywhere, Category = "World Setting")
	TSubclassOf<AEntityItem> DroppedItemClass;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 Seed = 0;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	EGenerationMethod ChunkGenerationMethod = EGenerationMethod::Greedy;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float RenderRate = 0.05f;

	// 每次渲染Chunk的个数
	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 RenderCount = 5;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float UnloadRate = 0.05f;

	// 每次卸载Chunk的个数
	UPROPERTY(EditAnywhere, Category = "World Setting")
	int32 UnloadCount = 5;

	UPROPERTY(EditAnywhere, Category = "World Setting")
	float UpdateRate = 1.0f;

	FTimerHandle RenderQueueHandle;
	FTimerHandle UpdateHandle;
	FTimerHandle UnloadHandle;

	std::atomic<uint64> m_Tick;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FChunkPos CenterChunkPos = { 1, 1 };

	/*
	* First Initializer
	*/
	int32 Total = 0;

	std::atomic<int32> Count = 0;

private:
	TArray<FIntPoint> ChunksToUpdate;
	FCriticalSection ChunkUpdateCritical;

	FWorldRunner* ChunkUpdateThread;
	FChunkTickRunner* ChunkTickThread;

public:
	FORCEINLINE UTerrainBase* GetTerrain() const { return TerrainBase; }
};
