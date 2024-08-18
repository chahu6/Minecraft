#pragma once

#include "CoreMinimal.h"

class AChunk;
/**
 *
 */
class MINECRAFT_API FChunkGeneratorAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FChunkGeneratorAsyncTask>;

public:
	FChunkGeneratorAsyncTask() = default;

	FChunkGeneratorAsyncTask(AChunk* Chunk);

	~FChunkGeneratorAsyncTask() = default;

public:
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FChunkGeneratorAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();

private:
	AChunk* Chunk = nullptr;
};
