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

bool UChunkManagerComponent::CreateChunk(const FIntPoint& ChunkVoxelPosition)
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
			Rebuild_Adjacent_Chunks(ChunkVoxelPosition);
			return true;
		}
	}
	return false;
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

void UChunkManagerComponent::Rebuild_Adjacent_Chunks(const FVector2D& ChunkVoxelWorldPosition)
{
	// X轴
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X - 1, ChunkVoxelWorldPosition.Y);
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X + 1, ChunkVoxelWorldPosition.Y);

	// Y轴
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X, ChunkVoxelWorldPosition.Y - 1);
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X, ChunkVoxelWorldPosition.Y + 1);

	//Z轴的不需要
}

void UChunkManagerComponent::Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y)
{
	AChunk* Chunk = GetChunk(FIntPoint(Chunk_World_X, Chunk_World_Y));

	if (Chunk == nullptr) return;

	Chunk->Dirty();
}
