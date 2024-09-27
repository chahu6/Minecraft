#include "World/Runnable/WorldRunner.h"
#include "World/WorldManager.h"

FWorldRunner::FWorldRunner(const FString& ThreadName, AWorldManager* Manager)
	:WorldManager(Manager),
	m_ThreadName(ThreadName),
	ThreadIns(FRunnableThread::Create(this, *m_ThreadName, 0, TPri_Normal)),
	m_ThreadID(ThreadIns->GetThreadID()),
	ThreadEvent(FPlatformProcess::GetSynchEventFromPool())
{ 
}

FWorldRunner::~FWorldRunner()
{
	if (ThreadEvent)
	{
		FPlatformProcess::ReturnSynchEventToPool(ThreadEvent);
		ThreadEvent = nullptr;
	}

	if (ThreadIns)
	{
		delete ThreadIns;
		ThreadIns = nullptr;
	}
}

void FWorldRunner::SuspendThread()
{
	bPause = true;
}

void FWorldRunner::WakeUpThread()
{
	bPause = false;
	ThreadEvent->Trigger();
}

void FWorldRunner::StopThread()
{
	Stop();
	WakeUpThread();

	if (ThreadIns)
	{
		ThreadIns->WaitForCompletion();
	}
}

void FWorldRunner::ShutDown(bool bShouldWait)
{
	if (ThreadIns)
	{
		ThreadIns->Kill(bShouldWait);
	}
}

bool FWorldRunner::Init()
{
	return true;
}

uint32 FWorldRunner::Run()
{
	while (bRun)
	{
		if (bPause)
		{
			ThreadEvent->Wait();
			if (!bRun) return 0;
		}

		WorldManager->ThreadUpdate();
	}
	return 0;
}

void FWorldRunner::Stop()
{
	bRun = false;
	bPause = false;
}

void FWorldRunner::Exit()
{
}
