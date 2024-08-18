#include "World/Runnable/ChunkGeneratorAsyncTask.h"
#include "Chunk/Chunk.h"
#include "World/WorldManager.h"

FChunkGeneratorAsyncTask::FChunkGeneratorAsyncTask(AChunk* Chunk)
{
	this->Chunk = Chunk;
}

void FChunkGeneratorAsyncTask::DoWork()
{
	if (!IsValid(Chunk))
	{
		return;
	}

	Chunk->BuildMesh();

	AWorldManager* WorldManager = Cast<AWorldManager>(Chunk->GetOwner());
	if (WorldManager)
	{
		WorldManager->TaskQueue.Enqueue(Chunk);
	}
}
