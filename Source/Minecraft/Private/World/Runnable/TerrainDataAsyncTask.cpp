#include "World/Runnable/TerrainDataAsyncTask.h"
#include "World/WorldManager.h"

void FTerrainDataAsyncTask::DoWork()
{
	if (WorldManager == nullptr) return;

	FIntPoint ChunkLoc;
	for (int32 ChunkX = -WorldManager->ChunkRenderRange; ChunkX <= WorldManager->ChunkRenderRange; ++ChunkX)
	{
		for (int32 ChunkY = -WorldManager->ChunkRenderRange; ChunkY <= WorldManager->ChunkRenderRange; ++ChunkY)
		{
			if (bIsStopped) return;

			ChunkLoc = WorldManager->CharacterChunkPosition + FIntPoint(ChunkX, ChunkY);
			WorldManager->LoadChunkInfo(ChunkLoc);
		}
	}

	WorldManager->RenderChunksAsync();
}