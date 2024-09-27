#include "World/Runnable/ChunkGeneratorAsyncTask.h"
#include "Chunk/Chunk.h"
#include "World/WorldManager.h"

FChunkGeneratorAsyncTask::FChunkGeneratorAsyncTask(AChunk* Chunk)
{
	this->Chunk = Chunk;
}

void FChunkGeneratorAsyncTask::Abandon()
{
	bIsStopped = true;

	if (Chunk)
	{
		Chunk->StopBuildMesh();
	}
}

void FChunkGeneratorAsyncTask::DoWork()
{
	if (!IsValid(Chunk))
	{
		return;
	}

	Chunk->BuildMesh();

	AWorldManager* WorldManager = Cast<AWorldManager>(Chunk->GetOwner());
	if (WorldManager && !bIsStopped)
	{
		//WorldManager->TaskQueue.Enqueue(Chunk);
	}
}
