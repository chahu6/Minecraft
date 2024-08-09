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
#if 0
	//WorldManager->UpdateWorldAsync();
	double OldTime = FPlatformTime::Seconds();
	double Result = 0;
	for (double i = 0; i < 1000000000; ++i)
	{
		Result += FMath::Sqrt(i) / 1000000000;
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), Result);
	double End = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Warning, TEXT("%u Second: %fms end"), __LINE__, (End - OldTime) * 1000);
#endif

	if (!IsValid(Chunk))
	{
		return;
	}

	FScopeLock RefreshLock(&Chunk->BuildDataMutex);

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
