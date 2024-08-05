#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ChunkInterface.h"
#include "Chunk.generated.h"

class ITerrainGenerator;
class AChunkSection;
class FWorldGeneratorAsyncTask;
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

	virtual void SetBlock(int32 X, int32 Y, int32 Z, const FBlockData& BlockData) override;

	void BuildAndRenderAsync();

	bool IsDone();

	void Load(ITerrainGenerator* Generator);

	void Render();

public:
	FCriticalSection BuildDataMutex;

private:
	UPROPERTY()
	TArray<AChunkSection*> ChunkSections;

	FAsyncTask<FWorldGeneratorAsyncTask>* WorldGeneratorTask = nullptr;

	int32 Seed = -1;

	FString SlotName;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE const TArray<AChunkSection*>& GetChunkSections() const { return ChunkSections; }
};
