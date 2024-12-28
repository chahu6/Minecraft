// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/ChunkPos.h"
#include "WorldProviderComponent.generated.h"

class AChunk;
class AWorldManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UWorldProviderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWorldProviderComponent();
	virtual void BeginPlay() override;

	void UpdateWorld();

	AChunk* GetChunk(const FChunkPos& InChunkPos) const;
	void RemoveChunk(const FChunkPos& InChunkPos);

private:
	void HandleChunks(TArray<FChunkPos>& GenerateChunksPos, TSet<FChunkPos>& LastChunksPos);

	AChunk* SpawnChunk(const FChunkPos& ChunkPos);

protected:
	UPROPERTY(EditAnywhere, Category = "World Setting", meta = (ClampMin = "0"))
	int16 LoadChunkDistance = 5;

	UPROPERTY(EditAnywhere, Category = "World Setting", meta = (ClampMin = "0"))
	int16 UnLoadChunkDistance = 7;

private:
	TWeakObjectPtr<AWorldManager> WorldManager;
	TMap<FChunkPos, AChunk*> Chunks;
};
