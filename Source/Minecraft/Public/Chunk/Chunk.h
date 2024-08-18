#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ChunkInterface.h"
#include "World/GenerationMethod.h"
#include "Chunk.generated.h"

struct FBlockData;
class UChunkMeshComponent;
struct FBlockPos;

UENUM()
enum class EChunkState : uint8
{
	None,
	Loaded,
	Rendered
};

UCLASS()
class MINECRAFT_API AChunk : public AActor, public IChunkInterface
{
	GENERATED_BODY()
	
public:	
	AChunk();

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	
	// 将所有的ChunkSection都设置为脏数据，Chunk是脏数据代表所属的ChunkSection也是脏数据
	void Dirty();

	FBlockData GetBlock(int32 OffsetX, int32 OffsetY, int32 WorldZ);

	FBlockData GetBlock(const FBlockPos& BlockPos);

	void SetBlock(int32 OffsetX, int32 OffsetY, int32 WorldZ, const FBlockData& BlockData);

	void BuildAndRender();

	void BuildAndRenderAsync();

	void RecalculateEmpty();

	void Render();

	void BuildMesh();

	EChunkState ChunkState = EChunkState::None;

protected:
	UPROPERTY(EditAnywhere, Category = "Setting")
	EGenerationMethod GenerationMethod = EGenerationMethod::Greedy;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChunkMeshComponent> ChunkMeshComponent;

	TArray<FBlockData> Blocks;

	bool bIsEmpty = false;

	TArray<int32> HeightMap;

	int32 Seed = -1;

	FString SlotName;

	bool bIsRendering = false;

	bool bIsDirty = false;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE TArray<int32>& GetHeightMap() { return HeightMap; }
	FORCEINLINE void SetGenerationMethod(EGenerationMethod Method) { GenerationMethod = Method; }
};
