#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Minecraft/MinecraftType/FaceType.h"
#include "Minecraft/MinecraftType/BlockType.h"
#include "Chunk.generated.h"

class ITerrainGenerator;

UCLASS()
class MINECRAFT_API AChunk : public AActor
{
	GENERATED_BODY()
	
	friend class UChunkMeshComponent;
public:	
	AChunk();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;

	uint8 GetBlock(int32 X, int32 Y, int32 Z);
	uint8 GetBlock(int32 Index);

	void SetBlock(int32 X, int32 Y, int32 Z, uint8 BlockID);
	void SetBlock(int32 Index, uint8 BlockID);

	void Render();

	void Load(ITerrainGenerator* Generator);

	void Rebuild();

	void RecalculateEmpty();
private:

private:
	UPROPERTY(VisibleAnywhere)
	class UChunkMeshComponent* ChunkMesh;

private:
	TArray<uint8> Blocks;

	int32 Seed = -1;
	FString SlotName;

	// 是否Blocks数据修改过
	bool bIsDirty = true;

	// 是否都是Air或None
	bool bIsEmpty = true;

	// Chunk在World中的体素位置的中心
	FVector Center;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE void SetDirty(bool bNewDirty) { bIsDirty = bNewDirty; }
	FORCEINLINE void SetEmpty(bool bNewEmpty) { bIsEmpty = bNewEmpty; }
	FORCEINLINE bool IsEmpty() { return bIsEmpty; }
	FORCEINLINE const FVector& GetCenter() const { return Center; }
};
