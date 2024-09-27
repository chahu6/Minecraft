#include "World/Runnable/ChunkGenerateRunner.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"
#include "World/Generator/GreedyMeshGenerator.h"
#include "World/Components/TerrainComponent.h"

FChunkGenerateRunner::FChunkGenerateRunner(const FString& ThreadName, AWorldManager* Manager)
	:WorldManager(Manager),
	m_ThreadName(ThreadName),
	ThreadIns(FRunnableThread::Create(this, *m_ThreadName, 0, EThreadPriority::TPri_Normal)),
	m_ThreadID(ThreadIns->GetThreadID()),
	ThreadEvent(FPlatformProcess::GetSynchEventFromPool())
{
}

FChunkGenerateRunner::~FChunkGenerateRunner()
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

void FChunkGenerateRunner::SuspendThread()
{
	bPause = true;
}

void FChunkGenerateRunner::WakeUpThread()
{
	bPause = false;
	ThreadEvent->Trigger();
}

void FChunkGenerateRunner::StopThread()
{
	Stop();
	WakeUpThread();

	if (ThreadIns)
	{
		ThreadIns->WaitForCompletion();
	}
}

void FChunkGenerateRunner::ShutDown(bool bShouldWait)
{
	if (ThreadIns)
	{
		ThreadIns->Kill(bShouldWait);
	}
}

bool FChunkGenerateRunner::Init()
{
	return true;
}

uint32 FChunkGenerateRunner::Run()
{
	while (bRun)
	{
		if (bPause)
		{
			ThreadEvent->Wait();
			if (!bRun) return 0;
		}

		if (CoordsChanged())
		{
			GenerateChunks();

			FPlatformProcess::Sleep(0.1f);

			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, *WorldManager->CharacterChunkPosition.ToString());
		}
	}
	return 0;
}

void FChunkGenerateRunner::Stop()
{
	bRun = false;
	bPause = false;
}

void FChunkGenerateRunner::Exit()
{
}

void FChunkGenerateRunner::GenerateChunks()
{
	TArray<FIntPoint> Locations;
	int32 CurrentRadius = 0;

	for (auto Itr = WorldManager->WorldInfo.MeshDataCache.CreateConstIterator(); Itr; ++Itr)
	{
		LastActiveLoc.Add(Itr->Key);
	}

	if (!WorldManager->WorldInfo.MeshDataCache.Contains(WorldManager->CharacterChunkPosition))
	{
		Locations.Add(WorldManager->CharacterChunkPosition);
	}
	if(LastActiveLoc.Contains(WorldManager->CharacterChunkPosition))
	{
		LastActiveLoc.Remove(WorldManager->CharacterChunkPosition);
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
			if (LastActiveLoc.Contains(ChunkVoxelPosition))
			{
				LastActiveLoc.Remove(ChunkVoxelPosition);
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
			if (LastActiveLoc.Contains(ChunkVoxelPosition))
			{
				LastActiveLoc.Remove(ChunkVoxelPosition);
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
			if (LastActiveLoc.Contains(ChunkVoxelPosition))
			{
				LastActiveLoc.Remove(ChunkVoxelPosition);
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
			if (LastActiveLoc.Contains(ChunkVoxelPosition))
			{
				LastActiveLoc.Remove(ChunkVoxelPosition);
			}
		}

		CurrentRadius++;
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		WorldManager->WorldInfo.ChunkDataMap.Add(Locations[i], MakeShared<FChunkData>(Locations[i]));
		WorldManager->WorldInfo.MeshDataCache.Add(Locations[i], {});
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		WorldManager->TerrainManager->LoadTerrainInfo(WorldManager, Locations[i]);
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		GreedyMeshGenerator::BuildGreedyChunkMesh(WorldManager->WorldInfo, Locations[i]);
	}

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		WorldManager->SpawnChunkQueue.Enqueue(Locations[i]);
	}

	for (int32 i = 0; i < LastActiveLoc.Num(); ++i)
	{
		WorldManager->UnloadChunkQueue.Enqueue(LastActiveLoc[i]);
	}

	LastActiveLoc.Empty();
}

bool FChunkGenerateRunner::CoordsChanged()
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
