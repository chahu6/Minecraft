#include "World/Runnable/ChunkLoadWork.h"
#include "World/WorldManager.h"
#include "World/Gen/TerrainBase.h"
#include "World/Gen/GreedyMeshGenerator.h"

FChunkLoadWork::FChunkLoadWork(AWorldManager* InWorldManager, const FChunkPos& InChunkPos)
	:WorldManager(InWorldManager),
	ChunkPos(InChunkPos)
{
}

void FChunkLoadWork::DoThreadedWork()
{
	WorldManager->GetTerrain()->Generate(WorldManager, ChunkPos);

	GreedyMeshGenerator::BuildGreedyChunkMesh(WorldManager->WorldInfo, ChunkPos);

	WorldManager->LoadChunkQueue.Enqueue(ChunkPos);

	Abandon();
}

void FChunkLoadWork::Abandon()
{
	delete this;
}
