#include "World/Components/ChunkPoolComponent.h"
#include "Chunk/Chunk.h"
#include "World/WorldGenerator.h"

UChunkPoolComponent::UChunkPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PooledActorClass = AChunk::StaticClass();
	PoolSize = 200;
}

void UChunkPoolComponent::BeginPlay()
{
	Super::BeginPlay();

}

AChunk* UChunkPoolComponent::SpawnChunk(const FChunkPos& ChunkPos)
{
	AChunk* Chunk = SpawnFromPool<AChunk>(FTransform(FVector(ChunkPos.X * WorldGenerator::ChunkSize, ChunkPos.Y * WorldGenerator::ChunkSize, 0)));
	if (!IsValid(Chunk))
	{
		Chunk = SpawnPooledActor<AChunk>();
		Chunk->SetActorTransform(FTransform(FVector(ChunkPos.X * WorldGenerator::ChunkSize, ChunkPos.Y * WorldGenerator::ChunkSize, 0)));
		//Chunk->SetInUse(true);
	}
	return Chunk;
}
