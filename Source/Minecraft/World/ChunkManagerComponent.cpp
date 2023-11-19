#include "ChunkManagerComponent.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/Generation/ClassicOverWorldGenerator.h"
#include "WorldManager.h"
#include "Minecraft/World/WorldSettings.h"

UChunkManagerComponent::UChunkManagerComponent()
{
	_TerrainGenerator = MakeUnique<FClassicOverWorldGenerator>();
}

AChunk* UChunkManagerComponent::GetChunk(const FVector& Key)
{
	if (_AllChunks.Contains(Key))
	{
		return _AllChunks[Key];
	}
	return nullptr;
}

void UChunkManagerComponent::LoadChunk(const FVector& ChunkVoxelPosition)
{
	AChunk* Chunk = GetChunk(ChunkVoxelPosition);
	if (Chunk == nullptr)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			Chunk = World->SpawnActor<AChunk>(AChunk::StaticClass(), ChunkVoxelPosition * ChunkSize, FRotator::ZeroRotator, SpawnParams);
			Chunk->Load(_TerrainGenerator.Get());
			_AllChunks.Add(ChunkVoxelPosition, Chunk);

			// 加载新的区块时，让新区块的四个面的区块也加载
			Rebuild_Adjacent_Chunks(ChunkVoxelPosition);
		}
	}
}

void UChunkManagerComponent::Rebuild_Adjacent_Chunks(const FVector& ChunkVoxelWorldPosition)
{
	// X轴
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X - 1, ChunkVoxelWorldPosition.Y, ChunkVoxelWorldPosition.Z);
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X + 1, ChunkVoxelWorldPosition.Y, ChunkVoxelWorldPosition.Z);

	// Y轴
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X, ChunkVoxelWorldPosition.Y - 1, ChunkVoxelWorldPosition.Z);
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X, ChunkVoxelWorldPosition.Y + 1, ChunkVoxelWorldPosition.Z);

	// Z轴因为是新区块，所以z轴的不需要
}

void UChunkManagerComponent::Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z)
{
	AChunk* Chunk = GetChunk(FVector(Chunk_World_X, Chunk_World_Y, Chunk_World_Z));

	if (Chunk == nullptr)
		return;

	if (!Chunk->IsEmpty())
		Chunk->SetDirty(true);
}
