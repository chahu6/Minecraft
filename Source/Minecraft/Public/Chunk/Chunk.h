#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ChunkInterface.h"
#include "Chunk.generated.h"

//struct FBlockData;
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

	void BuildMesh();

	void StopBuildMesh();

	void TickUpdate();

	void UpdateChunk();

	/**
	* new
	*/

	//void AddActiveVoxel(const FBlockData& BlockData);

	void RenderMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

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

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE void SetChunkState(EChunkState NewChunkState) { ChunkState = NewChunkState; }
	FORCEINLINE EChunkState GetChunkState() const { return ChunkState; }
};
