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
#include "Utils/ChunkHelper.h"
#include "World/Generator/GreedyMeshGenerator.h"

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
	GetWorldTimerManager().SetTimer(UnloadHandle, this, &AWorldManager::UnloadChunk, UnloadRate, true);
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
		FIntPoint ChunkVoxelPos;
		while (DirtyChunkQueue.Dequeue(ChunkVoxelPos) && WorldInfo.MeshDataCache.Contains(ChunkVoxelPos) && ActiveChunks.Contains(ChunkVoxelPos))
		{
			if (AChunk* ModifyChunk = ActiveChunks[ChunkVoxelPos])
			{
				ModifyChunk->RenderMesh(WorldInfo.MeshDataCache[ChunkVoxelPos]);
			}
		}
	}
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

bool AWorldManager::DestroyBlock(const FIntVector& BlockWorldVoxelLocation, bool bDropBlock)
{
	FVector WorldLocation = FVector(BlockWorldVoxelLocation * WorldSettings::BlockSize);
	WorldLocation.X += WorldSettings::BlockSize >> 1;
	WorldLocation.Y += WorldSettings::BlockSize >> 1;
	WorldLocation.Z += WorldSettings::BlockSize >> 1;

	FBlockState BlockState = GetBlockState(BlockWorldVoxelLocation);

	UBlock* Block = BlockState.GetBlock();

	Block->OnDestroy(WorldLocation);

	if (bDropBlock)
	{
		Block->DropBlockAsItem(this, BlockWorldVoxelLocation, 0);
	}

	SetBlockState(BlockWorldVoxelLocation, FBlockState(UBlocks::Air));

	return true;
}

void AWorldManager::PlaceBlock(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState)
{
	SetBlockState(BlockWorldVoxelLocation, BlockState);
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

		OffsetLocation.X %= WorldSettings::CHUNK_SIZE;
		OffsetLocation.Y %= WorldSettings::CHUNK_SIZE;
		//const int32 WorldZ = OffsetLocation.Z;

		ChunkData->SetBlockState(OffsetLocation, BlockState);

		AddChunkToUpdate(BlockWorldVoxelLocation);

		CheckSurroundingChunkNeedUpdate(OffsetLocation, ChunkVoxelLocationX, ChunkVoxelLocationY);
	}
}

void AWorldManager::CheckSurroundingChunkNeedUpdate(const FIntVector& BlockOffsetLocation, int32 ChunkVoxelLocationX, int32 ChunkVoxelLocationY)
{
	if (BlockOffsetLocation.X >= WorldSettings::CHUNK_SIZE - 1)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX + 1, ChunkVoxelLocationY));
	else if (BlockOffsetLocation.X <= 0)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX - 1, ChunkVoxelLocationY));
	if (BlockOffsetLocation.Y >= WorldSettings::CHUNK_SIZE - 1)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY + 1));
	else if (BlockOffsetLocation.Y <= 0)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY - 1));
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
		for (int32 I = 0; I < RenderCount; ++I) // 每帧最大能加载几个
		{
			if (SpawnChunkQueue.Dequeue(ChunkPos) && WorldInfo.MeshDataCache.Contains(ChunkPos))
			{
				AChunk* SpawnChunk = GetWorld()->SpawnActor<AChunk>(AChunk::StaticClass(), FVector(ChunkPos.X * WorldSettings::ChunkSize, ChunkPos.Y * WorldSettings::ChunkSize, 0), FRotator::ZeroRotator);
				if (SpawnChunk)
				{
					SpawnChunk->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					SpawnChunk->RenderMesh(WorldInfo.MeshDataCache[ChunkPos]);

					ActiveChunks.Add(ChunkPos, SpawnChunk);

					/*for (FBlockState VoxelState : WorldInfo.ChunkDataMap[ChunkPos]->BlockStateMap)
					{
						if (VoxelState.GetBlock() && VoxelState.GetBlock()->bIsActive)
						{
							WorldInfo.ChunkDataMap[ChunkPos]->ActiveVoxels.Add(VoxelState);
						}
					}*/
				}
			}
		}
	}
}

void AWorldManager::UnloadChunk()
{
	if (!UnloadChunkQueue.IsEmpty())
	{
		FIntPoint ChunkPos;
		for (int32 I = 0; I < UnloadCount; ++I) // 每帧最大能卸载几个Chunk
		{
			if (UnloadChunkQueue.Dequeue(ChunkPos))
			{
				if (ActiveChunks.Contains(ChunkPos))
				{
					ActiveChunks[ChunkPos]->Destroy();
					ActiveChunks.Remove(ChunkPos);
				}
				WorldInfo.MeshDataCache.Remove(ChunkPos);
				WorldInfo.ChunkDataMap.Remove(ChunkPos);
			}
		}
	}
}

void AWorldManager::AddChunkToUpdate(const FIntVector& BlockWorldVoxelLocation, bool bTop)
{
	AddChunkToUpdate(ChunkHelper::GetChunkVoxelFromBlockWorldVoxel(BlockWorldVoxelLocation));
}

void AWorldManager::AddChunkToUpdate(const FIntPoint& ChunkVoxelLocation, bool bTop)
{
	FScopeLock ChunkUpdateThreadLock(&ChunkUpdateCritical);

	if (!ChunksToUpdate.Contains(ChunkVoxelLocation))
	{
		if (bTop)
		{
			ChunksToUpdate.Insert(ChunkVoxelLocation, 0);
		}
		else
		{
			ChunksToUpdate.Add(ChunkVoxelLocation);
		}
		ChunkUpdateThread->WakeUpThread();
	}
}

void AWorldManager::ThreadUpdate()
{
	if (!ChunksToUpdate.IsEmpty())
	{
		UpdateChunks();
	}
	else
	{
		if (ChunkUpdateThread)ChunkUpdateThread->SuspendThread();
	}
}

void AWorldManager::UpdateChunks()
{
	FScopeLock ChunkUpdateThreadLock(&ChunkUpdateCritical);

	const FIntPoint ChunkVoxelLocation = ChunksToUpdate[0];
	if (ActiveChunks.Contains(ChunkVoxelLocation))
	{
		GreedyMeshGenerator::BuildGreedyChunkMesh(WorldInfo, ChunkVoxelLocation);
		DirtyChunkQueue.Enqueue(ChunkVoxelLocation);
	}
	ChunksToUpdate.RemoveAt(0);
}

void AWorldManager::UpdateChunk()
{
	for (const TPair<FIntPoint, AChunk*>& Pair : ActiveChunks)
	{
		WorldInfo.ChunkDataMap[Pair.Key]->TickUpdate();
	}
}
