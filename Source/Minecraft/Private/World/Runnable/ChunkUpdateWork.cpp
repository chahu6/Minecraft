#include "World/Runnable/ChunkUpdateWork.h"
#include "World/WorldManager.h"
#include "Chunk/Chunk.h"
#include "World/Gen/GreedyMeshGenerator.h"

FChunkUpdateWork::FChunkUpdateWork(AWorldManager* InWorldManager, const FChunkPos& InChunkPos)
	:WorldManager(InWorldManager),
	ChunkPos(InChunkPos)
{
}

void FChunkUpdateWork::DoThreadedWork()
{
	GreedyMeshGenerator::BuildGreedyChunkMesh(WorldManager->WorldInfo, ChunkPos);

	WorldManager->DirtyChunkQueue.Enqueue(ChunkPos);

	Abandon();
}

void FChunkUpdateWork::Abandon()
{
	delete this;
}
