#pragma once

#include "CoreMinimal.h"

class AChunk;
/**
 *
 */
class MINECRAFT_API FChunkGeneratorAsyncTask
{
	friend class FAsyncTask<FChunkGeneratorAsyncTask>;

public:
	FChunkGeneratorAsyncTask(AChunk* Chunk);

	bool CanAbandon()
	{
		return true;
	}

	void Abandon();

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FChunkGeneratorAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	AChunk* Chunk = nullptr;
	bool bIsStopped = false;
};
