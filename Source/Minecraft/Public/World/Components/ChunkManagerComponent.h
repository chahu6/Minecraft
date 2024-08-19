#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChunkManagerComponent.generated.h"

class AChunk;
class AWorldManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINECRAFT_API UChunkManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AWorldManager;

public:
	UChunkManagerComponent();

	virtual void BeginPlay() override;

	AChunk* GetChunk(const FIntPoint& Key);

	bool CreateChunk(const FIntPoint& ChunkVoxelPosition);

private:
	void Rebuild_Adjacent_Chunks(const FVector2D& ChunkVoxelWorldPosition);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y);

private:
	UPROPERTY()
	TMap<FIntPoint, AChunk*> AllChunks;

	UPROPERTY()
	TObjectPtr<AWorldManager> WorldManager;
};