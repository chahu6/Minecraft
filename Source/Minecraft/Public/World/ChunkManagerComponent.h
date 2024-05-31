#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChunkManagerComponent.generated.h"

class AWorldManager;
class AChunk;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINECRAFT_API UChunkManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AWorldManager;

public:
	UChunkManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AChunk* GetChunk(const FVector2D& Key);

	void LoadChunk(const FVector2D& ChunkVoxelPosition);

private:
	void Rebuild_Adjacent_Chunks(const FVector2D& ChunkVoxelWorldPosition);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y);

private:
	UPROPERTY()
	TMap<FVector2D, AChunk*> _AllChunks;

	UPROPERTY()
	class UClassicOverWorldGenerator* _TerrainGenerator;

};