#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Minecraft/MinecraftType/FaceType.h"
#include "Minecraft/MinecraftType/BlockType.h"
#include "Chunk.generated.h"

UCLASS()
class MINECRAFT_API AChunk : public AActor
{
	GENERATED_BODY()
	
	friend class AWorldManager;
	friend class UChunkMeshComponent;
public:	
	AChunk();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Load(class ITerrainGenerator* Generator);

	void DrawChunk();

	void SetBlock(int32 X, int32 Y, int32 Z, EBlockType BlockType);

	EBlockType GetBlock(int32 X, int32 Y, int32 Z);

private:
	UPROPERTY(VisibleAnywhere)
	class UChunkMeshComponent* ChunkMesh;

private:
	TArray<EBlockType> Blocks;

	int32 Seed = -1;
	FString SlotName;

	bool bIsChunkLoaded = false;
	bool bIsRendering = false;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
	FORCEINLINE bool HasLoaded() const noexcept { return bIsChunkLoaded; }
	FORCEINLINE bool HasRendering() const noexcept { return bIsRendering; }
};
