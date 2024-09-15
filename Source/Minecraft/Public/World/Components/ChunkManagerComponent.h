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

	void SpawnChunk(const FIntPoint& ChunkVoxelPosition);

	void EnsureCompletion();

private:
	UPROPERTY()
	TMap<FIntPoint, AChunk*> AllChunks;

	UPROPERTY()
	TObjectPtr<AWorldManager> WorldManager;

public:
	FORCEINLINE int32 GetAllChunksNum() const { return AllChunks.Num(); }
};