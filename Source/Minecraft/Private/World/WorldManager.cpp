#include "World/WorldManager.h"
#include "World/WorldSettings.h"
#include "Chunk/Chunk.h"
#include "World/Components/ChunkManagerComponent.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/MinecraftAssetLibrary.h"
#include "World/Behavior/BlockBehavior.h"
#include "World/Components/TerrainComponent.h"
#include "World/Runnable/WorldRunner.h"
#include "World/Runnable/ChunkGenerateRunner.h"
#include "Chunk/MeshData.h"

#include "World/Block/Blocks.h"

AWorldManager* AWorldManager::Instance = nullptr;

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkManager = CreateDefaultSubobject<UChunkManagerComponent>(TEXT("ChunkManagerComponent"));

	TerrainManager = CreateDefaultSubobject<UTerrainComponent>(TEXT("TerrainComponent"));
}

void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;

	ChunkUpdateThread = new FWorldRunner(TEXT("ChunkUpdateThread"), this);

	ChunkGenerateThread = new FChunkGenerateRunner(TEXT("TestThread"), this);

	USimplexNoiseLibrary::SetNoiseSeed(Seed);

	InitialWorldChunkLoad();

	GetWorldTimerManager().SetTimer(RenderQueueHandle, this, &AWorldManager::RenderChunk, RenderRate, true);
	GetWorldTimerManager().SetTimer(UpdateHandle, this, &AWorldManager::UpdateChunk, UpdateRate, true);
}

void AWorldManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ChunkUpdateThread)
	{
		ChunkUpdateThread->StopThread();
		delete ChunkUpdateThread;
		ChunkUpdateThread = nullptr;
	}

	if (ChunkGenerateThread)
	{
		ChunkGenerateThread->StopThread();
		delete ChunkGenerateThread;
		ChunkGenerateThread = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!DirtyChunkQueue.IsEmpty())
	{
		AChunk* DirtyChunk = nullptr;
		DirtyChunkQueue.Dequeue(DirtyChunk);
		if (DirtyChunk != nullptr)
		{
			//DirtyChunk->Render();
		}
	}

	/*if (UpdatePosition())
	{
		AddChunk();
		RemoveChunk();
	}*/
}

void AWorldManager::InitialWorldChunkLoad()
{
	//DefaultCharacterPosition = FVector2D(FMath::RandRange(-100.f, 100.f));
	//FVector2D NewLocation2D = DefaultCharacterPosition;

	//CharacterChunkPosition.X = FMath::FloorToInt32(NewLocation2D.X / WorldSettings::ChunkSize);
	//CharacterChunkPosition.Y = FMath::FloorToInt32(NewLocation2D.Y / WorldSettings::ChunkSize);
}

bool AWorldManager::UpdatePosition()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (Pawn == nullptr) return false;

	FIntPoint NewLocation2D;
	NewLocation2D.X = FMath::FloorToInt32(Pawn->GetActorLocation().X / WorldSettings::ChunkSize);
	NewLocation2D.Y = FMath::FloorToInt32(Pawn->GetActorLocation().Y / WorldSettings::ChunkSize);
	if (NewLocation2D != CharacterChunkPosition)
	{
		CharacterChunkPosition = NewLocation2D;
		return true;
	}

	return false;
}

bool AWorldManager::DestroyBlock(const FIntVector& BlockWorldVoxelLocation)
{
	FVector WorldLocation = FVector(BlockWorldVoxelLocation * WorldSettings::BlockSize);
	WorldLocation.X += WorldSettings::BlockSize >> 1;
	WorldLocation.Y += WorldSettings::BlockSize >> 1;
	WorldLocation.Z += WorldSettings::BlockSize >> 1;

	FBlockState BlockState = GetBlockState(BlockWorldVoxelLocation);
	if (BlockState.GetBlock())
	{
		BlockState.GetBlock()->OnDestroy(WorldLocation);
	}

	SetBlockState(BlockWorldVoxelLocation, FBlockState(UBlocks::Air));

	//AChunk* Chunk = GetChunk(BlockWorldVoxelLocation);
	//if (Chunk == nullptr) return false;

	//Chunk->Rebuild();
	//Rebuild_Adjacent_Chunks(BlockPos);

	//AddChunkToUpdate(Chunk);

	return true;
}

void AWorldManager::PlaceBlock(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState)
{
	SetBlockState(BlockWorldVoxelLocation, BlockState);

	/*AChunk* Chunk = GetChunk(BlockWorldVoxelLocation);
	if (Chunk == nullptr) return;

	AddChunkToUpdate(Chunk);*/
}

void AWorldManager::SetBlockState(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

	TSharedPtr<FChunkData> ChunkData = GetChunkData(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY));
	if (ChunkData)
	{
		FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldSettings::CHUNK_SIZE;

		const int32 OffsetX = OffsetLocation.X % WorldSettings::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldSettings::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;

		ChunkData->SetBlockState(OffsetX, OffsetY, WorldZ, BlockState);
	}
}

FBlockState AWorldManager::GetBlockState(const FIntVector& BlockWorldVoxelLocation)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

	TSharedPtr<FChunkData> ChunkData = GetChunkData(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY));
	if (ChunkData.IsValid())
	{
		FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldSettings::CHUNK_SIZE;

		const int32 OffsetX = OffsetLocation.X % WorldSettings::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldSettings::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;
		return ChunkData->GetBlockState(OffsetX, OffsetY, WorldZ);
	}

	return FBlockState();
}

TSharedPtr<FChunkData> AWorldManager::GetChunkData(const FIntPoint& ChunkVoxelLocation)
{
	if (WorldInfo.ChunkDataMap.Contains(ChunkVoxelLocation))
	{
		return WorldInfo.ChunkDataMap[ChunkVoxelLocation];
	}
	return nullptr;
}

void AWorldManager::RenderChunk()
{
	if (!SpawnChunkQueue.IsEmpty())
	{
		FIntPoint ChunkPos;
		if (SpawnChunkQueue.Dequeue(ChunkPos))
		{
			AChunk* SpawnChunk = GetWorld()->SpawnActor<AChunk>(AChunk::StaticClass(), FVector(ChunkPos.X * WorldSettings::ChunkSize, ChunkPos.Y * WorldSettings::ChunkSize, 0), FRotator::ZeroRotator);
			if (SpawnChunk)
			{
				SpawnChunk->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				SpawnChunk->RenderMesh(WorldInfo.MeshDataCache[ChunkPos]);

				ActiveChunks.Add(ChunkPos, SpawnChunk);
				
				for (FBlockState VoxelState : WorldInfo.ChunkDataMap[ChunkPos]->BlockStateMap)
				{
					if (VoxelState.GetBlock() && VoxelState.GetBlock()->bIsActive)
					{
						WorldInfo.ChunkDataMap[ChunkPos]->ActiveVoxels.Add(VoxelState);
					}
				}
			}
		}
	}
	else
	{
		if (!RemoveChunkQueue.IsEmpty())
		{
			FIntPoint ChunkPos;
			if (RemoveChunkQueue.Dequeue(ChunkPos))
			{
				if (ActiveChunks.Contains(ChunkPos))
				{
					ActiveChunks[ChunkPos]->Destroy();
				}
				WorldInfo.MeshDataCache.Remove(ChunkPos);
				WorldInfo.ChunkDataMap.Remove(ChunkPos);
				ActiveChunks.Remove(ChunkPos);
			}
		}
	}

	//if (TaskQueue.IsEmpty()) return;

	//AChunk* Chunk = nullptr;
	//for (int32 I = 0; I < RenderCount; ++I) // 每帧最大能加载几个
	//{
	//	if (TaskQueue.Dequeue(Chunk) && nullptr != Chunk)
	//	{
	//		Chunk->Render();
	//	}
	//}
}

void AWorldManager::AddChunkToUpdate(AChunk* Chunk, bool bTop)
{
	FScopeLock ChunkUpdateThreadLock(&ChunkUpdateCritical);

	if (!ChunksToUpdate.Contains(Chunk))
	{
		if (bTop)
		{
			ChunksToUpdate.Insert(Chunk, 0);
		}
		else
		{
			ChunksToUpdate.Add(Chunk);
		}
	}
}

void AWorldManager::ThreadedUpdate()
{
	if (!ChunksToUpdate.IsEmpty())
	{
		UpdateChunks();
	}
}

void AWorldManager::UpdateChunks()
{
	FScopeLock ChunkUpdateThreadLock(&ChunkUpdateCritical);

	ChunksToUpdate[0]->UpdateChunk();
	
	ChunksToUpdate.RemoveAt(0);
}

void AWorldManager::UpdateChunk()
{
	for (const TPair<FIntPoint, AChunk*>& Pair : ActiveChunks)
	{
		WorldInfo.ChunkDataMap[Pair.Key]->TickUpdate();
	}
}
