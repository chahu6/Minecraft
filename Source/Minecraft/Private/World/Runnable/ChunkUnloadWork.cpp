#include "World/Runnable/ChunkUnloadWork.h"
#include "World/WorldManager.h"

FChunkUnloadWork::FChunkUnloadWork(AWorldManager* InWorldManager, const FChunkPos& InChunkPos)
	:WorldManager(InWorldManager),
	ChunkPos(InChunkPos)
{
}

void FChunkUnloadWork::DoThreadedWork()
{
	WorldManager->UnloadChunkQueue.Enqueue(ChunkPos);

	Abandon();
}

void FChunkUnloadWork::Abandon()
{
	delete this;
}
