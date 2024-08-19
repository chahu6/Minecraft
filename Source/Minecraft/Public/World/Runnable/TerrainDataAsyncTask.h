#pragma once

#include "CoreMinimal.h"

class AChunk;
/**
 *
 */
//class MINECRAFT_API FTerrainDataAsyncTask : public FNonAbandonableTask
//{
//	friend class FAutoDeleteAsyncTask<FTerrainDataAsyncTask>;
//
//public:
//	FTerrainDataAsyncTask() = default;
//
//	FTerrainDataAsyncTask(AChunk* Chunk);
//
//	~FTerrainDataAsyncTask() = default;
//
//public:
//	FORCEINLINE TStatId GetStatId() const
//	{
//		RETURN_QUICK_DECLARE_CYCLE_STAT(FTerrainDataAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
//	}
//
//	void DoWork();
//
//private:
//	AChunk* Chunk = nullptr;
//};
