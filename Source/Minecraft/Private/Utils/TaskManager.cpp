#include "Utils/TaskManager.h"

FTaskManager* FTaskManager::TaskManager = nullptr;

FTaskManager::FTaskManager(float InExecuteIntervalTime)
{
    ExecuteIntervalTime = InExecuteIntervalTime;
    CurrentRemainderTime = InExecuteIntervalTime;
}

FTaskManager::FTaskManager()
{
    ExecuteIntervalTime = 0.0f;
    CurrentRemainderTime = ExecuteIntervalTime;
}

FTaskManager* FTaskManager::Get()
{
    if (nullptr == TaskManager)
    {
        TaskManager = new FTaskManager();
    }
    return TaskManager;
}

void FTaskManager::AddTask(FTask* Task)
{
    TaskQueue.Enqueue(Task);
}

void FTaskManager::ClearTaskQueue()
{
    while (!TaskQueue.IsEmpty())
    {
        FTask* Task = nullptr;
        if (TaskQueue.Dequeue(Task) && Task == nullptr)
        {
            delete Task;
        }
    }
}

void FTaskManager::Tick(float DeltaTime)
{
    if (bCanExcuteTask)
    {
        if (TaskQueue.IsEmpty())
        {
            bCanExcuteTask = false;
            return;
        }

        if (CurrentRemainderTime <= 0.f)
        {
            FTask* Task = nullptr;
            if (TaskQueue.Dequeue(Task) && nullptr != Task)
            {
                Task->DoTask();
                delete Task;
            }

            CurrentRemainderTime = ExecuteIntervalTime;
        }
        else
        {
            CurrentRemainderTime -= DeltaTime;
        }
    }
}

void FTaskManager::StartExcuteTask()
{
    bCanExcuteTask = true;
}

void FTaskManager::SetExcuteIntervalTime(float InExecuteIntervalTime)
{
    ExecuteIntervalTime = InExecuteIntervalTime;
}
