#include "World/Runnable/ChunkLoadWork.h"
#include "World/WorldManager.h"
#include "World/Gen/GreedyMeshGenerator.h"
#include "Chunk/Chunk.h"

FChunkLoadWork::FChunkLoadWork(AWorldManager* InWorldManager, const FChunkPos& InChunkPos)
	:WorldManager(InWorldManager),
	ChunkPos(InChunkPos)
{
}

void FChunkLoadWork::DoThreadedWork()
{
	if (WorldManager->GetRenderMethod() == EGenerationMethod::Normal)
	{
		GreedyMeshGenerator::BuildChunkMesh(WorldManager->WorldInfo, ChunkPos);
	}
	else
	{
		GreedyMeshGenerator::BuildGreedyChunkMesh(WorldManager->WorldInfo, ChunkPos);
	}

	WorldManager->LoadChunkQueue.Enqueue(ChunkPos);

	WorldManager->GetChunk(ChunkPos)->ThreadEvent->Trigger();

	Abandon();
}

void FChunkLoadWork::Abandon()
{
	delete this;
}
