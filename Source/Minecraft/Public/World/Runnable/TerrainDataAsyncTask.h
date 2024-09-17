#pragma once

#include "CoreMinimal.h"

class AWorldManager;
/**
 * 
 */
class FTerrainDataAsyncTask
{
	friend class FAsyncTask<FTerrainDataAsyncTask>;

public:
	void DoWork();

	FTerrainDataAsyncTask(AWorldManager* InWorldManager)
	{
		WorldManager = InWorldManager;
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTerrainDataAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	bool CanAbandon()
	{
		return true;
	}

	void Abandon()
	{
		bIsStopped = true;
	}

private:
	void LoadWorldMesh(const FIntPoint& OffsetPosition);

private:
	AWorldManager* WorldManager = nullptr;
	bool bIsStopped = false;
};
