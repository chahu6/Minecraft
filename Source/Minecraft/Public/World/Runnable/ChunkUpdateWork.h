#pragma once

#include "CoreMinimal.h"
#include "Math/ChunkPos.h"

class AWorldManager;

class FChunkUpdateWork : public IQueuedWork
{
public:
	FChunkUpdateWork(AWorldManager* InWorldManager, const FChunkPos& InChunkPos);

	virtual void DoThreadedWork() override;

	virtual void Abandon() override;

private:
	AWorldManager* WorldManager = nullptr;

	FChunkPos ChunkPos;
};