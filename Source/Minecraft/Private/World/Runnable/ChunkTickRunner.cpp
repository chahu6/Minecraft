// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Runnable/ChunkTickRunner.h"
#include "World/WorldManager.h"

FChunkTickRunner::FChunkTickRunner(const FString& ThreadName, AWorldManager* InWorldManager)
	:FCommonRunnable(ThreadName, InWorldManager)
{}

void FChunkTickRunner::DoThreadedWork()
{
	FPlatformProcess::Sleep(0.05f);

	if (WorldManager)
	{
		WorldManager->NextTick();
	}
}
