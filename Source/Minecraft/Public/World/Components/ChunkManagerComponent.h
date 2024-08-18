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

	AChunk* GetChunk(const FVector2D& Key);

	bool CreateChunk(const FVector2D& ChunkVoxelPosition);

	AChunk* operator[](const FVector2D& Position)
	{
		return AllChunks.FindChecked(Position);
	}

private:
	void Rebuild_Adjacent_Chunks(const FVector2D& ChunkVoxelWorldPosition);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y);

private:
	UPROPERTY()
	TMap<FVector2D, AChunk*> AllChunks;

	TArray<AChunk*> LoadingChunks;

	TArray<AChunk*> RenderingChunks;

	UPROPERTY()
	TObjectPtr<AWorldManager> WorldManager;

public:
	FORCEINLINE TMap<FVector2D, AChunk*>& GetAllChunks() { return AllChunks; }
};