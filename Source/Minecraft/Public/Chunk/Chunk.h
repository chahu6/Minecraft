#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "Interfaces/ChunkInterface.h"
#include "Math/ChunkPos.h"
#include "Chunk.generated.h"

//struct FBlockData;
struct FMeshData;
struct FChunkData;
class AWorldManager;
struct FBlockState;

UENUM()
enum class EChunkState : uint8
{
	None,
	Loaded,
	Rebuild,
	Rendered
};

UCLASS()
class MINECRAFT_API AChunk : public APooledActor, public IChunkInterface
{
	GENERATED_BODY()

	friend class UBlockMeshComponent;
	friend class UPlantMeshComponent;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;

public:
	AChunk();

	virtual void SetInUse(bool InUse) override;

	// 将所有的ChunkSection都设置为脏数据，Chunk是脏数据代表所属的ChunkSection也是脏数据
	void Dirty();

	void BuildMesh();

	void StopBuildMesh();

	void TickUpdate();

	void UpdateChunk();

	/**
	* new
	*/

	//void AddActiveVoxel(const FBlockData& BlockData);

	void RenderTerrainMesh();
	void RenderTerrainMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

	void RenderPlantMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

	void SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockState);

	IQueuedWork* MakeLoadWork();
	IQueuedWork* MakeUnLoadWork();

private:
	void AbandonWork();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBlockMeshComponent> BlockMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPlantMeshComponent> PlantMeshComponent;

	//TArray<FBlockData> ActiveVoxels;

	bool bIsEmpty = false;

	int32 Seed = -1;

	FString SlotName;

	bool bIsRendering = false;

	bool bIsDirty = false;

	bool bIsStopped = false;

	EChunkState ChunkState = EChunkState::None;

	TSharedPtr<FChunkData> ChunkData;

	TWeakObjectPtr<AWorldManager> WorldManager;

	FChunkPos ChunkPos;

	IQueuedWork* ThreadWork = nullptr;

public:
	FORCEINLINE void SetChunkData(const TSharedRef<FChunkData> NewChunkDaat) { ChunkData = NewChunkDaat; }
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE void SetChunkState(EChunkState NewChunkState) { ChunkState = NewChunkState; }
	FORCEINLINE EChunkState GetChunkState() const { return ChunkState; }
	FORCEINLINE void SetChunkPos(const FChunkPos& NewChunkPos) { ChunkPos = NewChunkPos; }
	FORCEINLINE FChunkPos GetChunkPos() const { return ChunkPos; }
};
