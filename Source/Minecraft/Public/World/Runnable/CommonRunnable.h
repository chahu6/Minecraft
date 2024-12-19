// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AWorldManager;

/**
 * 
 */
class MINECRAFT_API FCommonRunnable : public FRunnable
{
public:
	FCommonRunnable(const FString& InThreadName, AWorldManager* InWorldManager);
	~FCommonRunnable();

public:
	void SuspendThread();

	void WakeUpThread();

	void StopThread();

	void ShutDown(bool bShouldWait);

protected:
	virtual void DoThreadedWork() {};

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

protected:
	bool bRun = true;

	bool bPause = false;

	AWorldManager* WorldManager;

	FString m_ThreadName;

	FEvent* ThreadEvent;

	FRunnableThread* ThreadIns;

	uint32 m_ThreadID;
};
