#include "WorldManager.h"
#include "Minecraft/World/WorldSettings.h"
#include "Minecraft/Chunk/Chunk.h"
#include "ChunkManagerComponent.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "WorldRunner.h"
#include "WorldGeneratorAsyncTask.h"
#include "Minecraft/Core/BlockPos.h"

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = false;

	ChunkManager = CreateDefaultSubobject<UChunkManagerComponent>(TEXT("ChunkManagerComponent"));
}

void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	USimplexNoiseLibrary::SetNoiseSeed(Seed);

	InitialWorldChunkLoad();
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsUpdated = UpdatePosition();
	if (bIsUpdated)
	{
		AddChunk();
		RemoveChunk();
		RenderChunks();
	}
}

void AWorldManager::InitialWorldChunkLoad()
{
	for (int32 X = -ChunkRenderingRange; X <= ChunkRenderingRange; ++X)
	{
		for (int32 Y = -ChunkRenderingRange; Y <= ChunkRenderingRange; ++Y)
		{
			ChunkManager->LoadChunk(FVector2D(X, Y));
		}
	}

	RenderChunks();
}

bool AWorldManager::UpdatePosition()
{
	FVector2D NewLocation2D(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
	if (!UKismetMathLibrary::EqualEqual_Vector2DVector2D(CharacterPosition * ChunkSize, NewLocation2D, ChunkSize_Half))
	{
		CharacterPosition = NewLocation2D / ChunkSize;
		return true;
	}

	return false;
}

void AWorldManager::AddChunk()
{
	const int32 ChunkPositionX = CharacterPosition.X;
	const int32 ChunkPositionY = CharacterPosition.Y;

	const int32 MinRenderingRangeX = ChunkPositionX - ChunkRenderingRange;
	const int32 MaxRenderingRangeX = ChunkPositionX + ChunkRenderingRange;

	const int32 MinRenderingRangeY = ChunkPositionY - ChunkRenderingRange;
	const int32 MaxRenderingRangeY = ChunkPositionY + ChunkRenderingRange;

	for (int32 X = MinRenderingRangeX; X <= MaxRenderingRangeX; ++X)
	{
		for (int32 Y = MinRenderingRangeY; Y <= MaxRenderingRangeY; ++Y)
		{
			FVector2D ChunkPosition(X, Y);
			ChunkManager->LoadChunk(ChunkPosition);
		}
	}
}

void AWorldManager::RemoveChunk()
{
	const int32 ChunkPositionX = CharacterPosition.X;
	const int32 ChunkPositionY = CharacterPosition.Y;

	const int32 MinRenderingRangeX = ChunkPositionX - ChunkRenderingRange;
	const int32 MaxRenderingRangeX = ChunkPositionX + ChunkRenderingRange;

	const int32 MinRenderingRangeY = ChunkPositionY - ChunkRenderingRange;
	const int32 MaxRenderingRangeY = ChunkPositionY + ChunkRenderingRange;

	auto& ChunksMap = ChunkManager->_AllChunks;
	for (auto Itr = ChunksMap.CreateConstIterator(); Itr; ++Itr)
	{
		FVector2D ChunkLocation = Itr.Key();
		bool bIsRemove = MinRenderingRangeX <= ChunkLocation.X && ChunkLocation.X <= MaxRenderingRangeX && MinRenderingRangeY <= ChunkLocation.Y && ChunkLocation.Y <= MaxRenderingRangeY;
		if (!bIsRemove)
		{
			Itr.Value()->Destroy();
			ChunksMap.Remove(ChunkLocation);
		}
	}
}

void AWorldManager::RenderChunks()
{
	const auto& ChunksMap = ChunkManager->_AllChunks;
	for (const auto& Elem : ChunksMap)
	{
		Elem.Value->Render();
	}
}

AChunk* AWorldManager::GetChunk(const FVector2D& ChunkVoxelPosition)
{
	return ChunkManager->GetChunk(ChunkVoxelPosition);
}

AChunkSection* AWorldManager::GetChunkSection(const FVector& ChunkVoxelPosition)
{
	AChunk* Chunk = GetChunk(FVector2D(ChunkVoxelPosition));
	if (Chunk)
	{
		return Chunk->GetChunkSection(ChunkVoxelPosition.Z);
	}

	return nullptr;
}

AChunkSection* AWorldManager::GetChunkSection(const FBlockPos& BlockPos)
{
	int32 ChunkWorld_X = FMath::Floor(BlockPos.X_VOXEL_WORLD / CHUNK_SIZE);
	int32 ChunkWorld_Y = FMath::Floor(BlockPos.Y_VOXEL_WORLD / CHUNK_SIZE);
	int32 ChunkWorld_Z = FMath::Floor(BlockPos.Z_VOXEL_WORLD / CHUNK_SIZE);

	AChunk* Chunk = GetChunk(FVector2D(ChunkWorld_X, ChunkWorld_Y));
	if (Chunk)
	{
		return Chunk->GetChunkSection(ChunkWorld_Z);
	}

	return nullptr;
}

void AWorldManager::UpdateWorldAsync()
{

	//(new FAutoDeleteAsyncTask<FWorldGeneratorAsyncTask>(this))->StartBackgroundTask();

	/*bool bIsUpdated = UpdatePosition();
	if (bIsUpdated)
	{
		AddChunk();
		RemoveChunk();
		RenderChunks();
	}*/
}
