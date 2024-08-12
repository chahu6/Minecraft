#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ChunkInterface.h"
#include "Chunk.generated.h"

class AChunkSection;
class FChunkGeneratorAsyncTask;
struct FBlockData;

UCLASS()
class MINECRAFT_API AChunk : public AActor, public IChunkInterface
{
	GENERATED_BODY()
	
public:	
	AChunk();

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void Destroyed() override;

public:	
	AChunkSection* GetChunkSection(double Voxel_Z);

	// 将所有的ChunkSection都设置为脏数据，Chunk是脏数据代表所属的ChunkSection也是脏数据
	void Dirty();

	virtual FBlockData GetBlock(int32 X, int32 Y, int32 Z) override;

	virtual void SetBlock(int32 OffsetX, int32 OffsetY, int32 WorldZ, const FBlockData& BlockData) override;

	void BuildAndRender();

	void BuildAndRenderAsync();

	bool IsDone();

	void RecalculateEmpty();

	void Render();

	void BuildMesh();

private:
	UPROPERTY()
	TArray<AChunkSection*> ChunkSections;

	TArray<uint8> HeightMap;

	FAsyncTask<FChunkGeneratorAsyncTask>* ChunkGeneratorTask = nullptr;

	int32 Seed = -1;

	FString SlotName;

	bool bIsRendering = false;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE const TArray<AChunkSection*>& GetChunkSections() const { return ChunkSections; }
	FORCEINLINE TArray<uint8>& GetHeightMap() { return HeightMap; }
};
