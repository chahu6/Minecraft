#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

class ITerrainGenerator;
class AChunkSection;
class FWorldGeneratorAsyncTask;

UCLASS()
class MINECRAFT_API AChunk : public AActor
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

	// �����е�ChunkSection������Ϊ�����ݣ�Chunk�������ݴ���������ChunkSectionҲ��������
	void Dirty();

	uint8 GetBlock(int32 X, int32 Y, int32 Z);

	void SetBlock(int32 X, int32 Y, int32 Z, uint8 BlockID);

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
