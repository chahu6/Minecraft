#include "WorldRunner.h"
#include "Engine/Engine.h"
#include "WorldManager.h"

FWorldRunner::FWorldRunner(AWorldManager* WorldManager)
{
	this->WorldManager = WorldManager;

	WorldRunnerThread = FRunnableThread::Create(this, TEXT("WorldRunner"));
}

FWorldRunner::~FWorldRunner()
{

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
