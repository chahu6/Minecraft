#include "World/Runnable/WorldRunner.h"
#include "Engine/Engine.h"
#include "World/WorldManager.h"

FWorldRunner::FWorldRunner(AWorldManager* WorldManager)
{
	this->WorldManager = WorldManager;

	WorldRunnerThread = FRunnableThread::Create(this, TEXT("WorldRunner"));
}

FWorldRunner::~FWorldRunner()
{
	if (WorldRunnerThread)
	{
		delete WorldRunnerThread;
		WorldRunnerThread = nullptr;
	}
}

bool FWorldRunner::Init()
{
	return true;
}

uint32 FWorldRunner::Run()
{
	return 0;
}

void FWorldRunner::Stop()
{
}

void FWorldRunner::Exit()
{
}
