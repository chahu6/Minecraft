#include "World/Components/ChunkManagerComponent.h"
#include "Chunk/Chunk.h"
#include "World/WorldSettings.h"
#include "World/WorldManager.h"

UChunkManagerComponent::UChunkManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChunkManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
}

AChunk* UChunkManagerComponent::GetChunk(const FIntPoint& Key)
{
	if (AllChunks.Contains(Key))
	{
		return AllChunks[Key];
	}
	return nullptr;
}

void UChunkManagerComponent::SpawnChunk(const FIntPoint& ChunkVoxelPosition)
{
	AChunk* Chunk = GetChunk(ChunkVoxelPosition);
	if (Chunk == nullptr)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = WorldManager;
			Chunk = World->SpawnActorDeferred<AChunk>(AChunk::StaticClass(), FTransform(FVector(ChunkVoxelPosition.X * WorldSettings::ChunkSize, ChunkVoxelPosition.Y * WorldSettings::ChunkSize, 0.0)), WorldManager);
			Chunk->SetGenerationMethod(WorldManager->ChunkGenerationMethod);
			Chunk->FinishSpawning({}, true);
			AllChunks.Emplace(ChunkVoxelPosition, Chunk);

			// 加载新的区块时，让新区块的四个面的区块也加载
			//Rebuild_Adjacent_Chunks(ChunkVoxelPosition);
		}
	}
}

void UChunkManagerComponent::EnsureCompletion()
{
	for (auto Itr = AllChunks.CreateConstIterator(); Itr; ++Itr)
	{
		if (Itr->Value)
		{
			Itr->Value->EnsureCompletion();
		}
	}
}
