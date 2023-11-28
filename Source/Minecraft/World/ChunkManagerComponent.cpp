#include "ChunkManagerComponent.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/Generation/ClassicOverWorldGenerator.h"
#include "WorldManager.h"
#include "Minecraft/World/WorldSettings.h"

UChunkManagerComponent::UChunkManagerComponent()
{
	_TerrainGenerator = MakeUnique<FClassicOverWorldGenerator>();
}

AChunk* UChunkManagerComponent::GetChunk(const FVector2D& Key)
{
	if (_AllChunks.Contains(Key))
	{
		return _AllChunks[Key];
	}
	return nullptr;
}

void UChunkManagerComponent::LoadChunk(const FVector2D& ChunkVoxelPosition)
{
	AChunk* Chunk = GetChunk(ChunkVoxelPosition);
	if (Chunk == nullptr)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			Chunk = World->SpawnActor<AChunk>(AChunk::StaticClass(), { ChunkVoxelPosition.X * ChunkSize, ChunkVoxelPosition.Y * ChunkSize, 0.0 }, FRotator::ZeroRotator, SpawnParams);
			Chunk->Load(_TerrainGenerator.Get());
			_AllChunks.Add(ChunkVoxelPosition, Chunk);

			// 加载新的区块时，让新区块的四个面的区块也加载
			Rebuild_Adjacent_Chunks(ChunkVoxelPosition);
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
	AChunk* Chunk = GetChunk(FVector2D(Chunk_World_X, Chunk_World_Y));

	if (Chunk == nullptr)
		return;

	Chunk->Dirty();
}
