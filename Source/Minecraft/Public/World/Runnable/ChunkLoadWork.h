#pragma once

#include "CoreMinimal.h"
#include "Math/ChunkPos.h"

class AWorldManager;

class FChunkLoadWork : public IQueuedWork
{
public:
	FChunkLoadWork(AWorldManager* InWorldManager, const FChunkPos& InChunkPos);

	virtual void DoThreadedWork() override;

	virtual void Abandon() override;

private:
	AWorldManager* WorldManager = nullptr;

	FChunkPos ChunkPos;
};