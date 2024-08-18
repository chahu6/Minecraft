#pragma once

#include "CoreMinimal.h"

struct FTask
{
	FTask() = default;
	virtual ~FTask() = default;

	virtual void DoTask() = 0;
};

struct FTaskManager
{
public:
	FTaskManager(float InExecuteIntervalTime);
	FTaskManager();
	static FTaskManager* Get();

public:
	void AddTask(FTask* Task);
	void ClearTaskQueue();
	void Tick(float DeltaTime);
	void StartExcuteTask();
	void SetExcuteIntervalTime(float InExecuteIntervalTime = 0.0f);

private:
	TQueue<FTask*, EQueueMode::Mpsc> TaskQueue;

	float ExecuteIntervalTime;
	float CurrentRemainderTime;
	bool bCanExcuteTask = false;
	static FTaskManager* TaskManager;
};