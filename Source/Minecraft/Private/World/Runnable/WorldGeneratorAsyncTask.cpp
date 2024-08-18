#include "World/Runnable/WorldGeneratorAsyncTask.h"
#include "World/WorldManager.h"

FWorldGeneratorAsyncTask::FWorldGeneratorAsyncTask(AWorldManager* WorldManager)
{
	this->WorldManager = WorldManager;
}

void FWorldGeneratorAsyncTask::DoWork()
{
	if (!IsValid(WorldManager))return;

	WorldManager->RenderChunks();
}
