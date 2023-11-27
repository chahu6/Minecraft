#include "WorldManager.h"
#include "Minecraft/World/WorldSettings.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/Generation/ClassicOverWorldGenerator.h"
#include "ChunkManagerComponent.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;

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
			for (int32 Z = 0; Z < WORLD_HEIGHT; ++Z)
			{
				ChunkManager->LoadChunk(FVector(X, Y, Z));
			}
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
			for (int32 Z = 0; Z < WORLD_HEIGHT; ++Z)
			{
				FVector ChunkPosition(X, Y, Z);
				ChunkManager->LoadChunk(ChunkPosition);
			}
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
		FVector ChunkLocation = Itr.Key();
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
	auto& ChunksMap = ChunkManager->_AllChunks;
	for (auto& Elem : ChunksMap)
	{
		Elem.Value->Render();
	}
}

AChunk* AWorldManager::GetChunk(const FVector& ChunkVoxelPosition)
{
	return ChunkManager->GetChunk(ChunkVoxelPosition);
}
