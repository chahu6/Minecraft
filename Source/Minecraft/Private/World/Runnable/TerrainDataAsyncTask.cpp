#include "World/Runnable/TerrainDataAsyncTask.h"
#include "World/WorldManager.h"
#include "Chunk/Chunk.h"

void FTerrainDataAsyncTask::DoWork()
{
	if (WorldManager == nullptr) return;

	//FIntPoint ChunkLoc;
	//for (int32 ChunkX = -WorldManager->ChunkRenderRange; ChunkX <= WorldManager->ChunkRenderRange; ++ChunkX)
	//{
	//	for (int32 ChunkY = -WorldManager->ChunkRenderRange; ChunkY <= WorldManager->ChunkRenderRange; ++ChunkY)
	//	{
	//		if (bIsStopped) return;

	//		ChunkLoc = WorldManager->CharacterChunkPosition + FIntPoint(ChunkX, ChunkY);
	//		WorldManager->LoadChunkInfo(ChunkLoc);
	//	}
	//}

	WorldManager->LoadChunks();

	int32 CurrentRadius = 0;
	WorldManager->GetChunk(WorldManager->CharacterChunkPosition)->UpdateChunk();
	while (CurrentRadius <= WorldManager->LoadDistance)
	{
		// Forward
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkPosition = WorldManager->CharacterChunkPosition + FIntPoint(i, CurrentRadius);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		// Right
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkPosition = WorldManager->CharacterChunkPosition + FIntPoint(CurrentRadius, i);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		// Backward
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkPosition = WorldManager->CharacterChunkPosition + FIntPoint(i, -CurrentRadius);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		// Left
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkPosition = WorldManager->CharacterChunkPosition + FIntPoint(-CurrentRadius, i);
			WorldManager->GetChunk(ChunkPosition)->UpdateChunk();
		}

		CurrentRadius++;
	}

	//WorldManager->RenderChunksAsync();
}