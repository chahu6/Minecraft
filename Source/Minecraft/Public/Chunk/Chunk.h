#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ChunkInterface.h"
#include "World/GenerationMethod.h"
#include "Chunk.generated.h"

struct FBlockData;
class FChunkGeneratorAsyncTask;
struct FMeshData;

UENUM()
enum class EChunkState : uint8
{
	None,
	Loaded,
	Rebuild,
	Rendered
};

UCLASS()
class MINECRAFT_API AChunk : public AActor, public IChunkInterface
{
	GENERATED_BODY()

	friend class UBlockMeshComponent;
	friend class UPlantMeshComponent;
	
public:	
	AChunk();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// 将所有的ChunkSection都设置为脏数据，Chunk是脏数据代表所属的ChunkSection也是脏数据
	void Dirty();

	FBlockData GetBlock(int32 OffsetX, int32 OffsetY, int32 WorldZ);

	void SetBlock(int32 OffsetX, int32 OffsetY, int32 WorldZ, const FBlockData& BlockData);

	void BuildAndRender();

	void Rebuild();

	void BuildAndRenderAsync();

	void RecalculateEmpty();

	void BuildMesh();

	void Render();

	void EnsureCompletion();

	void StopBuildMesh();

	void TickUpdate();

	void UpdateChunk();

	/**
	* new
	*/

	void AddActiveVoxel(const FBlockData& BlockData);

	void RenderMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

protected:
	UPROPERTY(EditAnywhere, Category = "Setting")
	EGenerationMethod GenerationMethod = EGenerationMethod::Greedy;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBlockMeshComponent> BlockMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPlantMeshComponent> PlantMeshComponent;

	TArray<FBlockData> Blocks;

	//TArray<FBlockData> ActiveVoxels;

	bool bIsEmpty = false;

	TArray<int32> HeightMap;

	int32 Seed = -1;

	FString SlotName;

	bool bIsRendering = false;

	bool bIsDirty = false;

	FAsyncTask<FChunkGeneratorAsyncTask>* ChunkGeneratorTask = nullptr;

	bool bIsStopped = false;

	EChunkState ChunkState = EChunkState::None;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE TArray<int32>& GetHeightMap() { return HeightMap; }
	FORCEINLINE void SetGenerationMethod(EGenerationMethod Method) { GenerationMethod = Method; }
	FORCEINLINE void SetChunkState(EChunkState NewChunkState) { ChunkState = NewChunkState; }
	FORCEINLINE EChunkState GetChunkState() const { return ChunkState; }
};
