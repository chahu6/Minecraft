#include "World/Runnable/TestRunner.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"
#include "GreedyMeshGenerator.h"
#include "World/Components/TerrainComponent.h"

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
	while (bRun)
	{
		if (bPause)
		{
			ThreadEvent->Wait();
			if (!bRun) return 0;
		}

		if (UpdatePosition())
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, *WorldManager->CharacterChunkPosition.ToString());
			GenerateChunks();
		}
	}
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

void FTestRunner::GenerateChunks()
{
	TArray<FIntPoint> Locations;
	int32 CurrentRadius = 0;
	if (!WorldManager->WorldInfo.MeshDataCache.Contains(WorldManager->CharacterChunkPosition))
	{
		Locations.Add(WorldManager->CharacterChunkPosition);
	}
	while (CurrentRadius <= WorldManager->LoadDistance)
	{
		// Forward
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkVoxelPosition = WorldManager->CharacterChunkPosition + FIntPoint(i, CurrentRadius);
			if (!WorldManager->WorldInfo.MeshDataCache.Contains(ChunkVoxelPosition))
			{
				Locations.Add(ChunkVoxelPosition);
			}
		}

		// Right
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkVoxelPosition = WorldManager->CharacterChunkPosition + FIntPoint(CurrentRadius, i);
			if (!WorldManager->WorldInfo.MeshDataCache.Contains(ChunkVoxelPosition))
			{
				Locations.Add(ChunkVoxelPosition);
			}
		}

		// Backward
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkVoxelPosition = WorldManager->CharacterChunkPosition + FIntPoint(i, -CurrentRadius);
			if (!WorldManager->WorldInfo.MeshDataCache.Contains(ChunkVoxelPosition))
			{
				Locations.Add(ChunkVoxelPosition);
			}
		}

		// Left
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkVoxelPosition = WorldManager->CharacterChunkPosition + FIntPoint(-CurrentRadius, i);
			if (!WorldManager->WorldInfo.MeshDataCache.Contains(ChunkVoxelPosition))
			{
				Locations.Add(ChunkVoxelPosition);
			}
		}

		CurrentRadius++;
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		WorldManager->WorldInfo.ChunksMap.Add(Locations[i], ChunkData(Locations[i]));
		WorldManager->WorldInfo.MeshDataCache.Add(Locations[i], {});
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		WorldManager->TerrainManager->LoadTerrainInfo(WorldManager->WorldInfo, Locations[i]);
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		GreedyMeshGenerator::BuildGreedyChunkMesh(WorldManager->WorldInfo, Locations[i]);
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		WorldManager->SpawnChunkQueue.Enqueue(Locations[i]);
	}
}

bool FTestRunner::UpdatePosition()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldManager, 0);
	if (Pawn == nullptr) return false;

	FIntPoint NewLocation2D;
	NewLocation2D.X = FMath::FloorToInt32(Pawn->GetActorLocation().X / WorldSettings::ChunkSize);
	NewLocation2D.Y = FMath::FloorToInt32(Pawn->GetActorLocation().Y / WorldSettings::ChunkSize);
	if (NewLocation2D != WorldManager->CharacterChunkPosition)
	{
		WorldManager->CharacterChunkPosition = NewLocation2D;
		return true;
	}

	return false;
}
