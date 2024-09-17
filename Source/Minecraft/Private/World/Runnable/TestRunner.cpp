#include "World/Runnable/TestRunner.h"

FTestRunner::FTestRunner(const FString& ThreadName, AWorldManager* Manager)
	:WorldManager(Manager),
	m_ThreadName(ThreadName),
	ThreadIns(FRunnableThread::Create(this, *m_ThreadName, 0, EThreadPriority::TPri_Normal)),
	m_ThreadID(ThreadIns->GetThreadID()),
	ThreadEvent(FPlatformProcess::GetSynchEventFromPool())
{
}

FTestRunner::~FTestRunner()
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

void FTestRunner::SuspendThread()
{
	bPause = true;
}

void FTestRunner::WakeUpThread()
{
	bPause = false;
	ThreadEvent->Trigger();
}

void FTestRunner::StopThread()
{
	Stop();

	if (ThreadIns)
	{
		ThreadIns->WaitForCompletion();
	}
}

void FTestRunner::ShutDown(bool bShouldWait)
{
	if (ThreadIns)
	{
		ThreadIns->Kill(bShouldWait);
	}
}

bool FTestRunner::Init()
{
	return true;
}

uint32 FTestRunner::Run()
{

	return 0;
}

void FTestRunner::Stop()
{
	bRun = false;
	bPause = false;
}

void FTestRunner::Exit()
{
}
