#include "WorldGeneratorAsyncTask.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/Chunk/ChunkSection.h"

FWorldGeneratorAsyncTask::FWorldGeneratorAsyncTask(AChunk* Chunk)
{
	this->Chunk = Chunk;
}

void FWorldGeneratorAsyncTask::DoWork()
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

	for (const auto ChunkSection : ChunkSections)
	{
		ChunkSection->BuildMesh();
	}

	AsyncTask(ENamedThreads::GameThread, [Chunk = this->Chunk]()
	{
		if (Chunk)
		{
			Chunk->Render();
		}
	});
}
