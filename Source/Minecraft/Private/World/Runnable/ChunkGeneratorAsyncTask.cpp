#include "World/Runnable/ChunkGeneratorAsyncTask.h"
#include "Chunk/Chunk.h"
#include "Chunk/ChunkSection.h"
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

	const TArray<AChunkSection*>& ChunkSections = Chunk->GetChunkSections();

	for (AChunkSection* ChunkSection : ChunkSections)
	{
		ChunkSection->BuildMesh();
	}

	AWorldManager* WorldManager = Cast<AWorldManager>(Chunk->GetOwner());
	if (WorldManager)
	{
		WorldManager->TaskQueue.Enqueue(Chunk);
	}
}
