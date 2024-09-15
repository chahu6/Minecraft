#pragma once

#include "CoreMinimal.h"

class AWorldManager;
/**
 * 
 */
class MINECRAFT_API FWorldRunner : public FRunnable
{
public:
	FWorldRunner(const FString& ThreadName, AWorldManager* Manager);

	~FWorldRunner();

public:
	void SuspendThread();

	void WakeUpThread();

	void StopThread();

	void ShutDown(bool bShouldWait);

private:
	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

private:
	bool bRun = true;

	bool bPause = false;

	AWorldManager* WorldManager;

	FString m_ThreadName;

	FRunnableThread* ThreadIns;

	uint32 m_ThreadID;

	FEvent* ThreadEvent;
};
