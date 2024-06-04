#include "World/WorldManager.h"
#include "World/WorldSettings.h"
#include "Chunk/Chunk.h"
#include "World/ChunkManagerComponent.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/BlockPos.h"
#include "Generation/ClassicOverWorldGenerator.h"

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkManager = CreateDefaultSubobject<UChunkManagerComponent>(TEXT("ChunkManagerComponent"));

	TerrainGeneratorss = NewObject<UClassicOverWorldGenerator>(this, TEXT("TerrainGenerator"));
}

void AWorldManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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

	if (!TaskQueue.IsEmpty())
	{
		for (int32 i = 0; i < MAX_QUEUE_SIZE; ++i)
		{
			AChunk* Chunk = nullptr;

			if (!TaskQueue.Dequeue(Chunk)) break;

			if (Chunk)
			{
				Chunk->Render();
			}
		}
	}
}

void AWorldManager::InitialWorldChunkLoad()
{
	FVector2D NewLocation2D(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
	
	CharacterPosition.X = FMath::Floor(NewLocation2D.X / ChunkSize);
	CharacterPosition.Y = FMath::Floor(NewLocation2D.Y / ChunkSize);

	for (int32 X = -ChunkRenderingRange; X <= ChunkRenderingRange; ++X)
	{
		for (int32 Y = -ChunkRenderingRange; Y <= ChunkRenderingRange; ++Y)
		{
			FVector2D Temp = CharacterPosition + FVector2D(X, Y);
			ChunkManager->LoadChunk(Temp);
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

	auto& ChunksMap = ChunkManager->GetAllChunks();
	for (auto Itr = ChunksMap.CreateConstIterator(); Itr; ++Itr)
	{
		FVector2D ChunkLocation = Itr.Key();
		bool bIsRemove = MinRenderingRangeX <= ChunkLocation.X && ChunkLocation.X <= MaxRenderingRangeX && MinRenderingRangeY <= ChunkLocation.Y && ChunkLocation.Y <= MaxRenderingRangeY;
		if (!bIsRemove)
		{
			if (Itr.Value()->IsDone())
			{
				Itr.Value()->Destroy();
				ChunksMap.Remove(ChunkLocation);
			}
		}
	}
}

void AWorldManager::RenderChunks()
{
	const auto& ChunksMap = ChunkManager->GetAllChunks();
	for (const auto& Elem : ChunksMap)
	{
		Elem.Value->BuildAndRenderAsync();
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
