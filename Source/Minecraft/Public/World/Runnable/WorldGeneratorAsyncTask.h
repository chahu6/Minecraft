#pragma once

#include "CoreMinimal.h"

class AWorldManager;
/**
 * 
 */
class MINECRAFT_API FWorldGeneratorAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FWorldGeneratorAsyncTask>;

public:
	FWorldGeneratorAsyncTask() = default;

	FWorldGeneratorAsyncTask(AWorldManager* Chunk);

	~FWorldGeneratorAsyncTask() = default;

public:
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FWorldGeneratorAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();

private:
	AWorldManager* WorldManager = nullptr;
};
