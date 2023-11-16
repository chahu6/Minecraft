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

	void SetBlock(int32 X, int32 Y, int32 Z, uint8 BlockID);

	void Render();

	void Load(ITerrainGenerator* Generator);

	void BuildChunkMesh();

private:
	UPROPERTY(VisibleAnywhere)
	class UChunkMeshComponent* ChunkMesh;

private:
	TArray<uint8> Blocks;

	int32 Seed = -1;
	FString SlotName;

public:
	FORCEINLINE void SetSeed(int32 NewSeed) { Seed = NewSeed; }
};
