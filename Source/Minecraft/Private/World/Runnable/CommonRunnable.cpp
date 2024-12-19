// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Runnable/CommonRunnable.h"

FCommonRunnable::FCommonRunnable(const FString& InThreadName, AWorldManager* InWorldManager)
	:WorldManager(InWorldManager),
	m_ThreadName(InThreadName),
	ThreadEvent(FPlatformProcess::GetSynchEventFromPool()),
	ThreadIns(FRunnableThread::Create(this, *m_ThreadName, 0, EThreadPriority::TPri_Normal)),
	m_ThreadID(ThreadIns->GetThreadID())
{
}

FCommonRunnable::~FCommonRunnable()
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

void FCommonRunnable::SuspendThread()
{
	bPause = true;
}

void FCommonRunnable::WakeUpThread()
{
	bPause = false;
	ThreadEvent->Trigger();
}

void FCommonRunnable::StopThread()
{
	Stop();
	WakeUpThread();

	if (ThreadIns)
	{
		ThreadIns->WaitForCompletion();
	}
}

void FCommonRunnable::ShutDown(bool bShouldWait)
{
	if (ThreadIns)
	{
		ThreadIns->Kill(bShouldWait);
	}
}

bool FCommonRunnable::Init()
{
	return true;
}

uint32 FCommonRunnable::Run()
{
	while (bRun)
	{
		if (bPause)
		{
			ThreadEvent->Wait();
			if (!bRun) return 0;
		}

		DoThreadedWork();
	}
	return 0;
}

void FCommonRunnable::Stop()
{
	bRun = false;
	bPause = false;
}

void FCommonRunnable::Exit()
{
}
