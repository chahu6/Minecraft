#include "World/Runnable/TerrainDataAsyncTask.h"
#include "World/WorldManager.h"
#include "Chunk/Chunk.h"

void FTerrainDataAsyncTask::DoWork()
{
	if (WorldManager == nullptr) return;

	WorldManager->LoadChunks();

	LoadWorldMesh(WorldManager->CharacterChunkPosition);
}

void FTerrainDataAsyncTask::LoadWorldMesh(const FIntPoint& OffsetPosition)
{
	int32 CurrentRadius = 0;
	//WorldManager->GetChunk(OffsetPosition)->UpdateChunk();
	while (CurrentRadius <= WorldManager->LoadDistance)
	{
		if (bIsStopped) return;

		// Forward
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkPosition = OffsetPosition + FIntPoint(i, CurrentRadius);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		// Right
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkPosition = OffsetPosition + FIntPoint(CurrentRadius, i);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		// Backward
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkPosition = OffsetPosition + FIntPoint(i, -CurrentRadius);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		// Left
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkPosition = OffsetPosition + FIntPoint(-CurrentRadius, i);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		CurrentRadius++;
	}
}
