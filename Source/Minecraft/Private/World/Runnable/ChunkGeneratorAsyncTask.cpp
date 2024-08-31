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

	// 重新计算空值
	Chunk->RecalculateEmpty();

	Chunk->BuildMesh();

	AWorldManager* WorldManager = Cast<AWorldManager>(Chunk->GetOwner());
	if (WorldManager && !bIsStopped)
	{
		WorldManager->TaskQueue.Enqueue(Chunk);
	}
}
