#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkSection.generated.h"

class UChunkMeshComponent;
struct FBlockData;

UCLASS()
class MINECRAFT_API AChunkSection : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunkSection();

protected:
	virtual void PostInitializeComponents() override;

public:
	FBlockData GetBlock(int32 X, int32 Y, int32 Z) const;
	FBlockData GetBlock(int32 Index) const;

	void SetBlock(int32 X, int32 Y, int32 Z, const FBlockData& BlockData);
	void SetBlock(int32 Index, const FBlockData& BlockData);
	void SetBlock(const FVector& OffsetLocation, const FBlockData& BlockData);

	void BuildMesh();
	void Render();

	void Rebuild();

	void RecalculateEmpty();

private:
	UPROPERTY(VisibleAnywhere)
	UChunkMeshComponent* ChunkMesh;

private:
	TArray<FBlockData> Blocks;

	int32 Seed = -1;

	// 是否Blocks数据修改过
	bool bIsDirty = true;

	// 是否都是Air或None
	bool bIsEmpty = true;

	bool bIsReady = false;

	// Chunk在World中的体素位置的中心,用于视锥剔除，但是虚幻引擎默认自带，所以已经没用了
	FVector Center;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE void SetDirty(bool bNewDirty) { bIsDirty = bNewDirty; }
	FORCEINLINE void SetEmpty(bool bNewEmpty) { bIsEmpty = bNewEmpty; }
	FORCEINLINE bool IsEmpty() { return bIsEmpty; }
	FORCEINLINE const FVector& GetCenter() const { return Center; }

};
