#include "World/WorldManager.h"
#include "World/WorldGenerator.h"
#include "Chunk/Chunk.h"
#include "World/Components/ChunkPoolComponent.h"
#include "Kismet/GameplayStatics.h"
#include "World/Runnable/WorldRunner.h"
#include "Init/Blocks.h"
#include "Utils/ChunkHelper.h"
#include "World/Gen/GreedyMeshGenerator.h"
#include "Entity/Item/EntityItem.h"
#include "Interfaces/Block/TileEntityProvider.h"
#include "World/Runnable/ChunkTickRunner.h"
#include "World/Components/ChunkTaskPoolComponent.h"
#include "World/Gen/TerrainBase.h"
#include "World/Data/ChunkData.h"
#include "World/Components/WorldProviderComponent.h"
#include "Math/BlockPos.h"

AWorldManager* AWorldManager::Instance = nullptr;

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkPool = CreateDefaultSubobject<UChunkPoolComponent>(TEXT("ChunkPoolComponent"));
	ChunkTaskPool = CreateDefaultSubobject<UChunkTaskPoolComponent>(TEXT("ChunkTaskPoolComponent"));
	WorldProvider = CreateDefaultSubobject<UWorldProviderComponent>(TEXT("WorldProviderComponent"));
}

void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;

	checkf(TerrainBaseClass, TEXT("is not valid"));
	TerrainBase = NewObject<UTerrainBase>(this, TerrainBaseClass, FName(TEXT("TerrainBase")));
	TerrainBase->SetSeed(Seed);
	TerrainBase->BeginPlay();

	//ChunkUpdateThread = new FWorldRunner(TEXT("ChunkUpdateThread"), this);
	ChunkTickThread = new FChunkTickRunner(TEXT("ChunkTickThread"), this);

	GetWorldTimerManager().SetTimer(RenderQueueHandle, this, &AWorldManager::RenderChunk, RenderRate, true);
	GetWorldTimerManager().SetTimer(UnloadHandle, this, &AWorldManager::UnloadChunk, UnloadRate, true);
	GetWorldTimerManager().SetTimer(UpdateHandle, this, &AWorldManager::UpdateChunk, UpdateRate, true);

	InitialWorldChunkLoad();
}

void AWorldManager::InitialWorldChunkLoad()
{
	SetCenterPos(FChunkHelper::ChunkPosFromWorldLoc(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation()));
}

void AWorldManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ChunkUpdateThread)
	{
		ChunkUpdateThread->StopThread();
		delete ChunkUpdateThread;
		ChunkUpdateThread = nullptr;
	}
	if (ChunkTickThread)
	{
		ChunkTickThread->StopThread();
		delete ChunkTickThread;
		ChunkTickThread = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!DirtyChunkQueue.IsEmpty())
	{
		FChunkPos ChunkPos;
		while (DirtyChunkQueue.Dequeue(ChunkPos))
		{
			if (AChunk* Chunk = GetChunk(ChunkPos))
			{
				Chunk->RenderTerrainMesh();
			}
		}
	}

	CoordsChanged();

	const FVector ActorLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	const FChunkPos ChunkPos = FChunkHelper::ChunkPosFromWorldLoc(ActorLocation);
	const FBlockPos BlockPos = FChunkHelper::BlockPosFromWorldLoc(ActorLocation);
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("ChunkPos: %s"), *ChunkPos.ToString()));
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("BlockPos: %s"), *BlockPos.ToString()));
	const int32 TerrainHeight = GetHeight(BlockPos.X, BlockPos.Y);
	EBiomeID BiomeID = GetBiome(BlockPos);
	const UEnum* EnumObject = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBiomeID"));
	GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Red, FString::Printf(TEXT("TerrainHeight: %d"), TerrainHeight));
	GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Red, FString::Printf(TEXT("Biome: %s"), *EnumObject->GetMetaData(TEXT("DisplayName"), (int32)BiomeID)));

	TTuple<float, float, float, float, float> Noises;
	GetNoises(BlockPos, Noises);
	GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Red, FString::Printf(TEXT("Continent: %.3f"), Noises.Get<0>()));
	GEngine->AddOnScreenDebugMessage(7, 5.f, FColor::Red, FString::Printf(TEXT("Erosion: %.3f"), Noises.Get<1>()));
	GEngine->AddOnScreenDebugMessage(8, 5.f, FColor::Red, FString::Printf(TEXT("Peaks & Valleys: %.3f"), Noises.Get<2>()));
	GEngine->AddOnScreenDebugMessage(9, 5.f, FColor::Red, FString::Printf(TEXT("Temperature: %.3f"), Noises.Get<3>()));
	GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Red, FString::Printf(TEXT("Humidity: %.3f"), Noises.Get<4>()));
}

bool AWorldManager::CoordsChanged()
{
	if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		const FChunkPos NewCenterChunkPos = FChunkHelper::ChunkPosFromWorldLoc(Pawn->GetActorLocation());
		if (GetCenterChunkPos() != NewCenterChunkPos)
		{
			SetCenterPos(NewCenterChunkPos);
			return true;
		}
	}
	return false;
}

void AWorldManager::SetCenterPos(const FChunkPos& InChunkPos)
{
	if (InChunkPos == GetCenterChunkPos()) return;

	CenterChunkPos = InChunkPos;
	UpdateWorld();
}

void AWorldManager::UpdateWorld()
{
	WorldProvider->UpdateWorld();
}

bool AWorldManager::DestroyBlock(const FIntVector& BlockWorldVoxelLocation, bool bDropBlock)
{
	FVector WorldLocation = FVector(BlockWorldVoxelLocation * WorldGenerator::BlockSize);
	WorldLocation.X += WorldGenerator::BlockSize >> 1;
	WorldLocation.Y += WorldGenerator::BlockSize >> 1;
	WorldLocation.Z += WorldGenerator::BlockSize >> 1;

	FBlockState BlockState = GetBlockState(BlockWorldVoxelLocation);

	UBlock* Block = BlockState.GetBlock();

	Block->OnBlockDestroyedByPlayer(this, WorldLocation);

	if (bDropBlock)
	{
		Block->DropBlockAsItem(this, BlockWorldVoxelLocation, 0);
	}

	if (SetBlockState(BlockWorldVoxelLocation, FBlockState(UBlocks::Air)))
	{
		AddChunkToUpdate(FChunkHelper::ChunkPosFromBlockPos(BlockWorldVoxelLocation));

		CheckSurroundingChunkNeedUpdate(FChunkHelper::OffsetBlockPosFromBlockPos(BlockWorldVoxelLocation), FChunkHelper::ChunkPosFromBlockPos(BlockWorldVoxelLocation));
		return true;
	}
	return false;
}

bool AWorldManager::PlaceBlock(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState)
{
	if (SetBlockState(BlockWorldVoxelLocation, BlockState))
	{
		AddChunkToUpdate(FChunkHelper::ChunkPosFromBlockPos(BlockWorldVoxelLocation));

		CheckSurroundingChunkNeedUpdate(FChunkHelper::OffsetBlockPosFromBlockPos(BlockWorldVoxelLocation), FChunkHelper::ChunkPosFromBlockPos(BlockWorldVoxelLocation));
		return true;
	}
	return false;
}

AEntityItem* AWorldManager::SpawnEntity(const FVector& WorldLocation, const FItemStack& ItemStack)
{
	const double OffsetX = FMath::FRand() * 0.5f + 0.25;
	const double OffsetY = FMath::FRand() * 0.5f + 0.25;
	const double OffsetZ = FMath::FRand() * 0.5f + 0.25;

	FVector Location;
	Location.X = WorldLocation.X + OffsetX * 10.f;
	Location.Y = WorldLocation.Y + OffsetY * 10.f;
	Location.Z = WorldLocation.Z + OffsetZ * 10.f;

	checkf(DroppedItemClass, TEXT("DroppedItemClass Is Invalid!"));

	AEntityItem* EntityItem = GetWorld()->SpawnActorDeferred<AEntityItem>(DroppedItemClass, FTransform(Location), this);
	if (EntityItem)
	{
		EntityItem->SetItemStack(ItemStack);
		EntityItem->FinishSpawning({}, true);
		return EntityItem;
	}
	return nullptr;
}

AEntityItem* AWorldManager::SpawnEntity(const FIntVector& BlockWorldVoxelLocation, const FItemStack& ItemStack)
{
	const double OffsetX = FMath::FRand() * 0.5f + 0.25;
	const double OffsetY = FMath::FRand() * 0.5f + 0.25;
	const double OffsetZ = FMath::FRand() * 0.5f + 0.25;

	FVector ItemLocation;
	ItemLocation.X = BlockWorldVoxelLocation.X + OffsetX;
	ItemLocation.Y = BlockWorldVoxelLocation.Y + OffsetY;
	ItemLocation.Z = BlockWorldVoxelLocation.Z + OffsetZ;

	checkf(DroppedItemClass, TEXT("DroppedItemClass Is Invalid!"));

	AEntityItem* EntityItem = GetWorld()->SpawnActorDeferred<AEntityItem>(DroppedItemClass, FTransform(ItemLocation * WorldGenerator::BlockSize), this);
	if (EntityItem)
	{
		EntityItem->SetItemStack(ItemStack);
		EntityItem->FinishSpawning({}, true);
		return EntityItem;
	}
	return nullptr;
}

void AWorldManager::LoadChunks(const TArray<FChunkPos>& InChunksPos)
{
	for (const FChunkPos& ChunkPos : InChunksPos)
	{
		LoadChunk(ChunkPos);
	}
}

void AWorldManager::LoadChunk(const FChunkPos& InChunkPos)
{
	if (AChunk* Chunk = GetChunk(InChunkPos))
	{
		Chunk->AddQueuedWork<EChunkState::Load>(ChunkTaskPool->ChunkTaskPool);
	}
}

void AWorldManager::UnloadChunks(const TSet<FChunkPos>& InChunksPos)
{
	for (const FChunkPos& ChunkPos : InChunksPos)
	{
		UnloadChunk(ChunkPos);
	}
}

void AWorldManager::UnloadChunk(const FChunkPos& InChunkPos)
{
	if (AChunk* Chunk = GetChunk(InChunkPos))
	{
		Chunk->AddQueuedWork<EChunkState::Unload>(ChunkTaskPool->ChunkTaskPool);
	}
}

AChunk* AWorldManager::SpawnChunk(const FChunkPos& InChunkPos)
{
	return ChunkPool->SpawnChunk(InChunkPos);
}

FBlockPos AWorldManager::GetHeight(const FBlockPos& Pos)
{
	return FBlockPos(Pos.X, Pos.Y, GetHeight(Pos.X, Pos.Y) + Pos.Z);
}

int32 AWorldManager::GetHeight(int32 X, int32 Y)
{
	TSharedPtr<FChunkData> ChunkData = GetChunkData(X, Y);
	if (ChunkData.IsValid())
	{
		return ChunkData->GetHeight(X, Y);
	}
	return 0;
}

EBiomeID AWorldManager::GetBiome(const FBlockPos& Pos)
{
	TSharedPtr<FChunkData> ChunkData = GetChunkData(Pos);
	if (ChunkData.IsValid())
	{
		return ChunkData->GetBiome(Pos);
	}
	return EBiomeID::Ocean;
}

void AWorldManager::GetNoises(const FBlockPos& InBlockPos, TTuple<float, float, float, float, float>& OutNoises)
{
	TSharedPtr<FChunkData> ChunkData = GetChunkData(InBlockPos);
	if (ChunkData.IsValid())
	{
		ChunkData->GetNoises(InBlockPos, OutNoises);
	}
}

bool AWorldManager::SetBlockState(const FBlockPos& InBlockPos, const FBlockState& BlockState)
{
	return SetBlockState(InBlockPos.X, InBlockPos.Y, InBlockPos.Z, BlockState);
}

bool AWorldManager::SetBlockState(const FIntVector& BlockWorldVoxelLocation, const FBlockState& BlockState)
{
	return SetBlockState(BlockWorldVoxelLocation.X, BlockWorldVoxelLocation.Y, BlockWorldVoxelLocation.Z, BlockState);
}

bool AWorldManager::SetBlockState(int32 X, int32 Y, int32 Z, const FBlockState& BlockState)
{
	TSharedPtr<FChunkData> ChunkData = GetChunkData(FChunkHelper::ChunkPosFromBlockPos(X, Y));
	if (ChunkData.IsValid())
	{
		return ChunkData->SetBlockState(X, Y, Z, BlockState);
	}
	return false;
}

void AWorldManager::SetTileEntity(const FIntVector& BlockWorldVoxelLocation, ATileEntity* TileEntity)
{
	TileEntityList.Add(TileEntity);
}

void AWorldManager::CheckSurroundingChunkNeedUpdate(const FIntVector& BlockOffsetLocation, int32 ChunkVoxelLocationX, int32 ChunkVoxelLocationY)
{
	if (BlockOffsetLocation.X >= WorldGenerator::CHUNK_SIZE - 1)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX + 1, ChunkVoxelLocationY));
	else if (BlockOffsetLocation.X <= 0)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX - 1, ChunkVoxelLocationY));
	if (BlockOffsetLocation.Y >= WorldGenerator::CHUNK_SIZE - 1)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY + 1));
	else if (BlockOffsetLocation.Y <= 0)
		AddChunkToUpdate(FIntPoint(ChunkVoxelLocationX, ChunkVoxelLocationY - 1));
}

void AWorldManager::CheckSurroundingChunkNeedUpdate(const FBlockPos& InOffsetBlockPos, const FChunkPos& InChunkPos)
{
	if (InOffsetBlockPos.X >= WorldGenerator::CHUNK_SIZE - 1)
		AddChunkToUpdate(InChunkPos + FChunkPos(1, 0));
	else if (InOffsetBlockPos.X <= 0)
		AddChunkToUpdate(InChunkPos + FChunkPos(-1, 0));
	if (InOffsetBlockPos.Y >= WorldGenerator::CHUNK_SIZE - 1)
		AddChunkToUpdate(InChunkPos + FChunkPos(0, 1));
	else if (InOffsetBlockPos.Y <= 0)
		AddChunkToUpdate(InChunkPos + FChunkPos(0, -1));
}

FBlockState AWorldManager::GetBlockState(const FIntVector& BlockWorldVoxelLocation)
{
	return WorldInfo.GetBlockState(BlockWorldVoxelLocation);
}

FBlockState AWorldManager::GetBlockState(const FBlockPos& InBlockPos)
{
	return WorldInfo.GetBlockState(InBlockPos);
}

FBlockState AWorldManager::GetBlockState(int32 X, int32 Y, int32 Z)
{
	return WorldInfo.GetBlockState(X, Y, Z);
}

TSharedPtr<FChunkData> AWorldManager::GetChunkData(const FChunkPos& InChunkPos) const
{
	if (WorldInfo.ChunkDataMap.Contains(InChunkPos))
	{
		return WorldInfo.ChunkDataMap[InChunkPos];
	}
	return nullptr;
}

TSharedPtr<FChunkData> AWorldManager::GetChunkData(const FBlockPos& InBlockPos) const
{
	return GetChunkData(InBlockPos.X, InBlockPos.Y);
}

TSharedPtr<FChunkData> AWorldManager::GetChunkData(int32 X, int32 Y) const
{
	const FChunkPos ChunkPos = FChunkHelper::ChunkPosFromBlockPos(X, Y);
	return GetChunkData(ChunkPos);
}

AChunk* AWorldManager::GetChunk(const FChunkPos& InChunkPos) const
{
	return WorldProvider->GetChunk(InChunkPos);
}

void AWorldManager::RenderChunk()
{
	if (!LoadChunkQueue.IsEmpty())
	{
		for (int32 I = 0; I < RenderCount; ++I) // 每帧最大能加载几个
		{
			FChunkPos ChunkPos;
			if (LoadChunkQueue.Dequeue(ChunkPos))
			{
				if (AChunk* Chunk = GetChunk(ChunkPos))
				{
					Chunk->RenderTerrainMesh();
				}
			}
		}
	}
}

void AWorldManager::UnloadChunk()
{
	if (!UnloadChunkQueue.IsEmpty())
	{
		FChunkPos ChunkPos;
		for (int32 I = 0; I < UnloadCount; ++I) // 每帧最大能卸载几个Chunk
		{
			if (UnloadChunkQueue.Dequeue(ChunkPos))
			{
				RemoveChunk(ChunkPos);
			}
		}
	}
}

void AWorldManager::RemoveChunk(const FChunkPos& InChunkPos)
{
	WorldProvider->RemoveChunk(InChunkPos);
	WorldInfo.Remove(InChunkPos);
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

void AWorldManager::AddChunkToUpdate(const FChunkPos& InChunkPos, bool bTop)
{
	if (AChunk* Chunk = GetChunk(InChunkPos))
	{
		Chunk->AddQueuedWork<EChunkState::Update>(ChunkTaskPool->ChunkTaskPool);
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
	/*if (ActiveChunks.Contains(ChunkVoxelLocation))
	{
		GreedyMeshGenerator::BuildGreedyChunkMesh(WorldInfo, ChunkVoxelLocation);
		DirtyChunkQueue.Enqueue(ChunkVoxelLocation);
	}*/
	ChunksToUpdate.RemoveAt(0);
}

void AWorldManager::UpdateChunk()
{
	/*for (const TPair<FIntPoint, AChunk*>& Pair : ActiveChunks)
	{
		WorldInfo.ChunkDataMap[Pair.Key]->TickUpdate();
	}*/
}
