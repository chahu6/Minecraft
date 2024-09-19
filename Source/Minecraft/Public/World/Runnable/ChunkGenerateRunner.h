#pragma once

#include "CoreMinimal.h"

class AWorldManager;
/**
 *
 */
class MINECRAFT_API FChunkGenerateRunner : public FRunnable
{
public:
	FChunkGenerateRunner(const FString& ThreadName, AWorldManager* Manager);

	~FChunkGenerateRunner();

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

	void GenerateChunks();

	bool CoordsChanged();

private:
	bool bRun = true;

	bool bPause = false;

	AWorldManager* WorldManager;

	FString m_ThreadName;

	FRunnableThread* ThreadIns;

	uint32 m_ThreadID;

	FEvent* ThreadEvent;

	TArray<FIntPoint> LastActiveLoc;
};
