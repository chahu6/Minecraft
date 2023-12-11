#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkSection.generated.h"

UCLASS()
class MINECRAFT_API AChunkSection : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunkSection();

protected:
	virtual void PostInitializeComponents() override;

public:
	uint8 GetBlock(int32 X, int32 Y, int32 Z) const;
	uint8 GetBlock(int32 Index) const;

	void SetBlock(int32 X, int32 Y, int32 Z, uint8 BlockID);
	void SetBlock(int32 Index, uint8 BlockID);
	void SetBlock(const FVector& OffsetLocation, uint8 BlockID);

	void Render();

	void Rebuild();

	void RecalculateEmpty();

private:
	UPROPERTY(VisibleAnywhere)
	class UChunkMeshComponent* ChunkMesh;

private:
	TArray<uint8> Blocks;

	int32 Seed = -1;

	// 是否Blocks数据修改过
	bool bIsDirty = true;

	// 是否都是Air或None
	bool bIsEmpty = true;

	// Chunk在World中的体素位置的中心,用于视锥剔除，但是虚幻引擎默认自带，所以已经没用了
	FVector Center;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE void SetDirty(bool bNewDirty) { bIsDirty = bNewDirty; }
	FORCEINLINE void SetEmpty(bool bNewEmpty) { bIsEmpty = bNewEmpty; }
	FORCEINLINE bool IsEmpty() { return bIsEmpty; }
	FORCEINLINE const FVector& GetCenter() const { return Center; }

};
