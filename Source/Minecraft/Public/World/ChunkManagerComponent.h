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

public:
	UChunkManagerComponent();

	virtual void BeginPlay() override;

	AChunk* GetChunk(const FVector2D& Key);

	void LoadChunk(const FVector2D& ChunkVoxelPosition);

private:
	void Rebuild_Adjacent_Chunks(const FVector2D& ChunkVoxelWorldPosition);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y);

private:
	UPROPERTY()
	TMap<FVector2D, AChunk*> _AllChunks;

	UPROPERTY()
	TObjectPtr<AWorldManager> WorldManager;

public:
	FORCEINLINE TMap<FVector2D, AChunk*>& GetAllChunks() { return _AllChunks; }
};