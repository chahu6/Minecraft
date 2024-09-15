#include "World/WorldManager.h"
#include "World/WorldSettings.h"
#include "Chunk/Chunk.h"
#include "World/Components/ChunkManagerComponent.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/MinecraftAssetLibrary.h"
#include "World/Block/Block.h"
#include "World/Behavior/BlockBehavior.h"
#include "World/Runnable/TerrainDataAsyncTask.h"
#include "World/Components/TerrainComponent.h"
#include "World/Runnable/WorldRunner.h"

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

	USimplexNoiseLibrary::SetNoiseSeed(Seed);

	InitialWorldChunkLoad();

	GetWorldTimerManager().SetTimer(RenderQueueHandle, this, &AWorldManager::RenderChunk, RenderRate, true);
	//GetWorldTimerManager().SetTimer(UpdateHandle, this, &AWorldManager::UpdateChunk, UpdateRate, true);
}

void AWorldManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TerrainDataAsyncTask)
	{
		TerrainDataAsyncTask->TryAbandonTask();
		TerrainDataAsyncTask->EnsureCompletion();
		delete TerrainDataAsyncTask;
		TerrainDataAsyncTask = nullptr;
	}

	if (ChunkManager)
	{
		ChunkManager->EnsureCompletion();
	}

	if (ChunkUpdateThread)
	{
		ChunkUpdateThread->StopThread();
		delete ChunkUpdateThread;
		ChunkUpdateThread = nullptr;
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
			DirtyChunk->Render();
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
	DefaultCharacterPosition = FVector2D(FMath::RandRange(-100.f, 100.f));
	FVector2D NewLocation2D = DefaultCharacterPosition;

	CharacterChunkPosition.X = FMath::FloorToInt32(NewLocation2D.X / WorldSettings::ChunkSize);
	CharacterChunkPosition.Y = FMath::FloorToInt32(NewLocation2D.Y / WorldSettings::ChunkSize);


	int32 CurrentRadius = 0;
	SpawnChunk(CharacterChunkPosition);
	while (CurrentRadius <= LoadDistance)
	{
		// Forward
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkPosition = CharacterChunkPosition + FIntPoint(i, CurrentRadius);
			SpawnChunk(ChunkPosition);
		}

		// Right
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkPosition = CharacterChunkPosition + FIntPoint(CurrentRadius, i);
			SpawnChunk(ChunkPosition);
		}

		// Backward
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			FIntPoint ChunkPosition = CharacterChunkPosition + FIntPoint(i, -CurrentRadius);
			SpawnChunk(ChunkPosition);
		}

		// Left
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			FIntPoint ChunkPosition = CharacterChunkPosition + FIntPoint(-CurrentRadius, i);
			SpawnChunk(ChunkPosition);
		}

		CurrentRadius++;
	}

	//{
	//	// 初始化
	//	SpawnChunk(CharacterChunkPosition);
	//	LoadChunkInfo(CharacterChunkPosition);
	//	AChunk* Chunk = ChunkManager->GetChunk(CharacterChunkPosition);
	//	if (IsValid(Chunk))
	//	{
	//		Chunk->BuildAndRender();
	//	}
	//}

	//FIntPoint ChunkPosition;
	//for (int32 ChunkX = -ChunkRenderRange; ChunkX <= ChunkRenderRange; ++ChunkX)
	//{
	//	for (int32 ChunkY = -ChunkRenderRange; ChunkY <= ChunkRenderRange; ++ChunkY)
	//	{
	//		ChunkPosition = CharacterChunkPosition + FIntPoint(ChunkX, ChunkY);
	//		SpawnChunk(ChunkPosition);
	//	}
	//}

	TerrainDataAsyncTask = new FAsyncTask<FTerrainDataAsyncTask>(this);
	TerrainDataAsyncTask->StartBackgroundTask();
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

void AWorldManager::AddChunk()
{
	const int32 ChunkPositionX = CharacterChunkPosition.X;
	const int32 ChunkPositionY = CharacterChunkPosition.Y;

	const int32 MinRenderingRangeX = ChunkPositionX - ChunkRenderRange;
	const int32 MaxRenderingRangeX = ChunkPositionX + ChunkRenderRange;

	const int32 MinRenderingRangeY = ChunkPositionY - ChunkRenderRange;
	const int32 MaxRenderingRangeY = ChunkPositionY + ChunkRenderRange;

	for (int32 X = MinRenderingRangeX; X <= MaxRenderingRangeX; ++X)
	{
		for (int32 Y = MinRenderingRangeY; Y <= MaxRenderingRangeY; ++Y)
		{
			SpawnChunk(FIntPoint(X, Y));
		}
	}

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [=]() {
		for (int32 X = MinRenderingRangeX; X <= MaxRenderingRangeX; ++X)
		{
			for (int32 Y = MinRenderingRangeY; Y <= MaxRenderingRangeY; ++Y)
			{
				LoadChunkInfo(FIntPoint(X, Y));
			}
		}

		RenderChunksAsync();
	});
}

void AWorldManager::SpawnChunk(const FIntPoint& ChunkPosition)
{
	ChunkManager->SpawnChunk(ChunkPosition);
}

void AWorldManager::LoadChunkInfo(const FIntPoint& ChunkPosition)
{
	AChunk* Chunk = ChunkManager->GetChunk(ChunkPosition);
	if (Chunk && Chunk->GetChunkState() == EChunkState::None)
	{
		TerrainManager->LoadTerrainInfo(Chunk);
	}
}

void AWorldManager::RemoveChunk()
{
	const int32 ChunkPositionX = CharacterChunkPosition.X;
	const int32 ChunkPositionY = CharacterChunkPosition.Y;

	const int32 MinRenderingRangeX = ChunkPositionX - ChunkRenderRange;
	const int32 MaxRenderingRangeX = ChunkPositionX + ChunkRenderRange;

	const int32 MinRenderingRangeY = ChunkPositionY - ChunkRenderRange;
	const int32 MaxRenderingRangeY = ChunkPositionY + ChunkRenderRange;

	TMap<FIntPoint, AChunk*>& ChunksMap = ChunkManager->AllChunks;
	for (auto Itr = ChunksMap.CreateConstIterator(); Itr; ++Itr)
	{
		FIntPoint ChunkLocation = Itr.Key();
		bool bIsRemove = MinRenderingRangeX <= ChunkLocation.X && ChunkLocation.X <= MaxRenderingRangeX && MinRenderingRangeY <= ChunkLocation.Y && ChunkLocation.Y <= MaxRenderingRangeY;
		if (!bIsRemove)
		{
			Itr.Value()->Destroy();
			ChunksMap.Remove(ChunkLocation);
		}
	}
}

void AWorldManager::RenderChunksAsync()
{
	const TMap<FIntPoint, AChunk*>& ChunksMap = ChunkManager->AllChunks;
	for (const TPair<FIntPoint, AChunk*>& Elem : ChunksMap)
	{
		Elem.Value->BuildAndRenderAsync();
	}
}

AChunk* AWorldManager::GetChunk(const FIntPoint& ChunkVoxelLocation)
{
	return ChunkManager->GetChunk(ChunkVoxelLocation);
}

AChunk* AWorldManager::GetChunk(const FIntVector& BlockWorldVoxelLocation)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

	return GetChunk(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY));
}

bool AWorldManager::DestroyBlock(const FIntVector& BlockWorldVoxelLocation)
{
	FBlockData BlockData = GetBlock(BlockWorldVoxelLocation);
	if (!BlockData.IsValid()) return false;

	FBlockMeta BlockMeta;
	if (!UMinecraftAssetLibrary::GetBlockMeta(BlockData.BlockID(), BlockMeta)) return false;

	FVector WorldLocation = FVector(BlockWorldVoxelLocation * WorldSettings::BlockSize);
	WorldLocation.X += WorldSettings::BlockSize >> 1;
	WorldLocation.Y += WorldSettings::BlockSize >> 1;
	WorldLocation.Z += WorldSettings::BlockSize >> 1;

	if (BlockMeta.BehaviorClass) BlockMeta.BehaviorClass->GetDefaultObject<UBlockBehavior>()->OnDestroy(this, WorldLocation, BlockMeta.DestroySound);
	SetBlock(BlockWorldVoxelLocation, {});

	AChunk* Chunk = GetChunk(BlockWorldVoxelLocation);
	if (Chunk == nullptr) return false;

	//Chunk->Rebuild();
	//Rebuild_Adjacent_Chunks(BlockPos);

	AddChunkToUpdate(Chunk);

	return true;
}

void AWorldManager::PlaceBlock(const FIntVector& BlockWorldVoxelLocation, int32 BlockID)
{
	SetBlock(BlockWorldVoxelLocation, BlockID);

	AChunk* Chunk = GetChunk(BlockWorldVoxelLocation);
	if (Chunk == nullptr) return;

	Chunk->Rebuild();
}

void AWorldManager::SetBlock(const FIntVector& BlockWorldVoxelLocation, int32 BlockID)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

	AChunk* Chunk = GetChunk(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY));
	if (Chunk)
	{
		FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldSettings::CHUNK_SIZE;

		const int32 OffsetX = OffsetLocation.X % WorldSettings::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldSettings::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;

		Chunk->SetBlock(OffsetX, OffsetY, WorldZ, { static_cast<EBlockID>(BlockID), 0 });
	}
}

FBlockData AWorldManager::GetBlock(const FIntVector& BlockWorldVoxelLocation)
{
	// static_cast<float>() 转成float考虑到负数
	const int32 ChunkVoxelLocationX = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.X) / WorldSettings::CHUNK_SIZE);
	const int32 ChunkVoxelLocationY = FMath::FloorToInt32(static_cast<float>(BlockWorldVoxelLocation.Y) / WorldSettings::CHUNK_SIZE);

	AChunk* Chunk = GetChunk(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY));

	if (Chunk)
	{
		FIntVector OffsetLocation = BlockWorldVoxelLocation - FIntVector(ChunkVoxelLocationX, ChunkVoxelLocationY, 0) * WorldSettings::CHUNK_SIZE;

		const int32 OffsetX = OffsetLocation.X % WorldSettings::CHUNK_SIZE;
		const int32 OffsetY = OffsetLocation.Y % WorldSettings::CHUNK_SIZE;
		const int32 WorldZ = OffsetLocation.Z;
		return Chunk->GetBlock(OffsetX, OffsetY, WorldZ);
	}

	return FBlockData();
}

//void AWorldManager::Rebuild_Adjacent_Chunks(const FBlockPos& BlockPos)
//{
//	// 获取Voxel位置
//	int32 Voxel_Local_X = BlockPos.X_OFFSET;
//	int32 Voxel_Local_Y = BlockPos.Y_OFFSET;
//	int32 Voxel_Local_Z = BlockPos.Z_OFFSET;
//
//	// 获取Chunk所在Voxel位置
//	int32 Chunk_World_X = FMath::Floor(BlockPos.X_VOXEL_WORLD / CHUNK_SIZE);
//	int32 Chunk_World_Y = FMath::Floor(BlockPos.Y_VOXEL_WORLD / CHUNK_SIZE);
//	int32 Chunk_World_Z = FMath::Floor(BlockPos.Z_VOXEL_WORLD / CHUNK_SIZE);
//
//	// X轴
//	if (Voxel_Local_X == 0)
//	{
//		Rebuild_Adj_Chunk(Chunk_World_X - 1, Chunk_World_Y, Chunk_World_Z);
//	}
//	else if (Voxel_Local_X == CHUNK_SIZE - 1)
//	{
//		Rebuild_Adj_Chunk(Chunk_World_X + 1, Chunk_World_Y, Chunk_World_Z);
//	}
//
//	// Y轴
//	if (Voxel_Local_Y == 0)
//	{
//		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y - 1, Chunk_World_Z);
//	}
//	else if (Voxel_Local_Y == CHUNK_SIZE - 1)
//	{
//		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y + 1, Chunk_World_Z);
//	}
//
//	// Z轴
//	if (Voxel_Local_Z == 0)
//	{
//		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y, Chunk_World_Z - 1);
//	}
//	else if (Voxel_Local_Z == CHUNK_SIZE - 1)
//	{
//		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y, Chunk_World_Z + 1);
//	}
//}

void AWorldManager::Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z)
{
	//AChunkSection* ChunkSection = GetChunkSection(FVector(Chunk_World_X, Chunk_World_Y, Chunk_World_Z));

	//if (ChunkSection == nullptr)
		return;

	//ChunkSection->Rebuild();
}

void AWorldManager::RenderChunk()
{
	if (TaskQueue.IsEmpty()) return;

	AChunk* Chunk = nullptr;
	for (int32 I = 0; I < RenderCount; ++I) // 每帧最大能加载几个
	{
		if (TaskQueue.Dequeue(Chunk) && nullptr != Chunk)
		{
			Chunk->Render();
		}
	}
}

void AWorldManager::LoadChunks()
{
	for (const TPair<FIntPoint, AChunk*>& Pair : ChunkManager->AllChunks)
	{
		AChunk* Chunk = Pair.Value;
		if (Chunk && Chunk->GetChunkState() == EChunkState::None)
		{
			TerrainManager->LoadTerrainInfo(Chunk);
		}
	}
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
