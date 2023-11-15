#include "WorldManager.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/Chunk/ChunkManager.h"
#include "Minecraft/Utils/Utils.h"
#include "Minecraft/MinecraftType/BlockType.h"

AWorldManager::AWorldManager()
	:ChunkManager(this)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 设置随机数种子
	USimplexNoiseLibrary::SetNoiseSeed(Seed);

	InitialWorldChunkLoad();
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsUpdated = UpdateLocation();
	if (bIsUpdated)
	{
		UpdateChunks();
		RenderingChunks();
	}
}

int32 AWorldManager::GetChunkNum() const
{
	return ChunkManager.AllChunks.Num();
}

bool AWorldManager::IsCreateFaceInDirection(EFaceType FaceType, const FVector2D& ChunkPosition, int32 X, int32 Y, int32 Z)
{
	FVector2D NearChunkPosition;

	switch (FaceType)
	{
	case EFaceType::Forward:
		NearChunkPosition = (ChunkPosition / 1600) + FVector2D(1.0, 0.0);
		break;
	case EFaceType::BackGround:
		NearChunkPosition = (ChunkPosition / 1600) + FVector2D(-1.0, 0.0);
		break;
	case EFaceType::Left:
		NearChunkPosition = (ChunkPosition / 1600) + FVector2D(0.0, -1.0);
		break;
	case EFaceType::Right:
		NearChunkPosition = (ChunkPosition / 1600) + FVector2D(0.0, 1.0);
		break;
	}

	int32 Temp_X = (X + 16) % 16;
	int32 Temp_Y = (Y + 16) % 16;
	int32 Temp_Z = (Z + 256) % 256;

	if (ChunkManager.AllChunks.Contains(NearChunkPosition))
	{
		AChunk* Chunk = *ChunkManager.AllChunks.Find(NearChunkPosition);
		if (Chunk->Blocks[GetBlocksIndex(Temp_X, Temp_Y, Temp_Z)] == EBlockType::Air)
		{
			return true;
		}
	}

	return false;
}

EBlockType AWorldManager::GetBlockTypeFromPosition(int32 X, int32 Y, int32 Z)
{
	return ChunkManager.GetBlockType(X, Y, Z);
}

bool AWorldManager::UpdateLocation()
{
	FVector2D NewPlayerLocation2D = FVector2D(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());

	if (!UKismetMathLibrary::EqualEqual_Vector2DVector2D(ChunkLocation * ChunkSize, NewPlayerLocation2D, 800.0f))
	{
		ChunkLocation = (NewPlayerLocation2D / ChunkSize);
		return true;
	}

	return false;
}

void AWorldManager::InitialWorldChunkLoad()
{
	UpdateChunks();
	RenderingChunks();
}

void AWorldManager::UpdateChunks()
{
	float Chunk_X = ChunkLocation.X;
	float Chunk_Y = ChunkLocation.Y;
	float Range = RenderingChunkScale;

	int32 Max_X = Chunk_X + Range;
	int32 Max_Y = Chunk_Y + Range;
	for (int32 X = Chunk_X - Range; X <= Max_X; ++X)
	{
		for (int32 Y = Chunk_Y - Range; Y <= Max_Y; ++Y)
		{
			ChunkManager.LoadChunk(X * ChunkSize, Y * ChunkSize);
		}
	}
}

void AWorldManager::RenderingChunks()
{
	FVector2D Location = ChunkLocation;
	for (auto Itr = ChunkManager.AllChunks.CreateIterator(); Itr; ++Itr)
	{
		double Distance = UKismetMathLibrary::Distance2D(Location * ChunkSize, Itr.Key());
		if (Distance > RenderingChunkScale * ChunkSize)
		{
			Itr.Value()->Destroy();
			ChunkManager.AllChunks.Remove(Itr.Key());
			continue;
		}
		else
		{
			if (Itr.Value()->HasRendering())
				continue;
			if (Itr.Key() == FVector2D(1600.0f))
			{
				UE_LOG(LogTemp, Warning, TEXT("fffffff"));
			}
			Itr.Value()->DrawChunk();
		}
	}
}
