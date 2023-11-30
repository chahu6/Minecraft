#pragma once

#include "CoreMinimal.h"

class AWorldManager;
/**
 * 
 */
class MINECRAFT_API FWorldGeneratorAsyncTask : public FNonAbandonableTask
{
public:
	FWorldGeneratorAsyncTask(AWorldManager* WorldManager);
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
