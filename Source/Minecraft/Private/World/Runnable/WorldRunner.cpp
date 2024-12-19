#include "World/Runnable/WorldRunner.h"
#include "World/WorldManager.h"

FWorldRunner::FWorldRunner(const FString& InThreadName, AWorldManager* InWorldManager)
	:FCommonRunnable(InThreadName, InWorldManager)
{}

void FWorldRunner::DoThreadedWork()
{
	WorldManager->ThreadUpdate();
}
