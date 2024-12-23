#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "Interfaces/ChunkInterface.h"
#include "Math/ChunkPos.h"
#include "World/Runnable/ChunkLoadWork.h"
#include "World/Runnable/ChunkUnloadWork.h"
#include "Chunk.generated.h"

struct FMeshData;
struct FChunkData;
class AWorldManager;
struct FBlockState;

UENUM()
enum class EChunkState : uint8
{
	None,
	Load,
	Unload
};

UCLASS()
class MINECRAFT_API AChunk : public APooledActor, public IChunkInterface
{
	GENERATED_BODY()

	friend class UBlockMeshComponent;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AChunk();

	/** ObjectPool Unload Function */
	virtual void SetInUse(bool InUse) override;

	// 将所有的ChunkSection都设置为脏数据，Chunk是脏数据代表所属的ChunkSection也是脏数据
	void Dirty();

	void BuildMesh();

	void TickUpdate();

	void UpdateChunk();

	/**
	* new
	*/

	void RenderTerrainMesh();
	void RenderTerrainMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

	void SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockState);

	template<EChunkState InChunkState>
	void AddQueuedWork(FQueuedThreadPool* ThreadPool)
	{
		switch (InChunkState)
		{
			case EChunkState::Load:
			{
				ChunkState = InChunkState;
				LoadWork = new FChunkLoadWork(WorldManager->Get(), ChunkPos);
				ThreadPool->AddQueuedWork(LoadWork);
				break;
			}
			case EChunkState::Unload:
			{
				if (ChunkState == EChunkState::Load)
				{
					ChunkState = InChunkState;
					if (ThreadPool->RetractQueuedWork(LoadWork))
					{
						ThreadEvent->Trigger();
					}
					ThreadPool->AddQueuedWork(new FChunkUnloadWork(WorldManager->Get(), ChunkPos));
				}
				break;
			}
		}
	}

private:
	/** 用于对象池的对象的初始化，相当于Actor的 BeginPlay()函数*/
	void OnLoadChunk();
	/** 用于对象池的对象的卸载，相当于Actor的 EndPlay()函数*/
	void OnUnloadChunk();

public:
	FEvent* ThreadEvent;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBlockMeshComponent> BlockMeshComponent;

	bool bIsEmpty = false;

	FString SlotName;

	bool bIsRendering = false;

	bool bIsDirty = false;

	EChunkState ChunkState = EChunkState::None;

	TSharedPtr<FChunkData> ChunkData;
	TWeakObjectPtr<AWorldManager> WorldManager;

	FChunkPos ChunkPos;
	IQueuedWork* LoadWork;

public:
	FORCEINLINE void SetChunkData(const TSharedRef<FChunkData> NewChunkDaat) { ChunkData = NewChunkDaat; }
	FORCEINLINE void SetChunkState(EChunkState NewChunkState) { ChunkState = NewChunkState; }
	FORCEINLINE EChunkState GetChunkState() const { return ChunkState; }
	FORCEINLINE void SetChunkPos(const FChunkPos& NewChunkPos) { ChunkPos = NewChunkPos; }
	FORCEINLINE FChunkPos GetChunkPos() const { return ChunkPos; }
};
