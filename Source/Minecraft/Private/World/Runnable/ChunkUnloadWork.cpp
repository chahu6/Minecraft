#include "World/Runnable/ChunkUnloadWork.h"
#include "World/WorldManager.h"
#include "Chunk/Chunk.h"

FChunkUnloadWork::FChunkUnloadWork(AWorldManager* InWorldManager, const FChunkPos& InChunkPos)
	:WorldManager(InWorldManager),
	ChunkPos(InChunkPos)
{
}

void FChunkUnloadWork::DoThreadedWork()
{
	WorldManager->GetChunk(ChunkPos)->ThreadEvent->Wait();

	WorldManager->UnloadChunkQueue.Enqueue(ChunkPos);

	Abandon();
}

void FChunkUnloadWork::Abandon()
{
	delete this;
}
