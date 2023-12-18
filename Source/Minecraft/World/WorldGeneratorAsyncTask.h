#pragma once

#include "CoreMinimal.h"

class AChunk;
/**
 * 
 */
class MINECRAFT_API FWorldGeneratorAsyncTask : public FNonAbandonableTask
{
	friend class FAsyncTask<FWorldGeneratorAsyncTask>;

public:
	FWorldGeneratorAsyncTask() = default;
	FWorldGeneratorAsyncTask(AChunk* Chunk);
	~FWorldGeneratorAsyncTask() = default;

public:
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FWorldGeneratorAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();

private:
	AChunk* Chunk = nullptr;
};
