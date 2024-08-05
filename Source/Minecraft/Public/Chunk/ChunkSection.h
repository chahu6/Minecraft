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

	// �Ƿ�Blocks�����޸Ĺ�
	bool bIsDirty = true;

	// �Ƿ���Air��None
	bool bIsEmpty = true;

	bool bIsReady = false;

	// Chunk��World�е�����λ�õ�����,������׶�޳��������������Ĭ���Դ��������Ѿ�û����
	FVector Center;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE void SetDirty(bool bNewDirty) { bIsDirty = bNewDirty; }
	FORCEINLINE void SetEmpty(bool bNewEmpty) { bIsEmpty = bNewEmpty; }
	FORCEINLINE bool IsEmpty() { return bIsEmpty; }
	FORCEINLINE const FVector& GetCenter() const { return Center; }

};
