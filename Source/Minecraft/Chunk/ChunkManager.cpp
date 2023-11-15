#include "ChunkManager.h"
#include "Chunk.h"
#include "Minecraft/Generation/ClassicOverWorldGenerator.h"
#include "Minecraft/WorldManager/WorldManager.h"

FChunkManager::FChunkManager(AWorldManager* _WorldManager)
	:WorldManager(_WorldManager)
{
	TerrainGenerator = MakeUnique<FClassicOverWorldGenerator>(WorldManager->GetSeed());
}

void FChunkManager::LoadChunk(int32 X, int32 Y)
{
	GetChunk(X, Y)->Load(TerrainGenerator.Get());
}

void FChunkManager::UnLoadChunk(int32 X, int32 Y)
{
	FVector2D Key(X, Y);
	if (AllChunks.Contains(Key))
	{
		AllChunks[Key]->Destroy();
		AllChunks.Remove(Key);
	}
}

AChunk* FChunkManager::GetChunk(int32 X, int32 Y)
{
	FVector2D Key(X, Y);
	if (!AllChunks.Contains(Key))
	{
		UWorld* World = WorldManager->GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = WorldManager;
			AChunk* Chunk = World->SpawnActor<AChunk>({ Key.X, Key.Y, 0.0f }, FRotator::ZeroRotator, SpawnParams);
			if (Chunk)
			{
				AllChunks.Add(Key, Chunk);
			}
		}
	}
	return AllChunks[Key];
}

EBlockType FChunkManager::GetBlockType(int32 X, int32 Y, int32 Z)
{
	int32 Sign_X = FMath::Sign(X);
	int32 Sign_Y = FMath::Sign(Y);

	int32 Temp_X = (FMath::Abs(X) + 800) / ChunkSize;
	int32 Temp_Y = (FMath::Abs(Y) + 800) / ChunkSize;

	Temp_X = Sign_X * Temp_X;
	Temp_Y = Sign_Y * Temp_Y;

	FVector2D Key(Temp_X * ChunkSize, Temp_Y * ChunkSize);
	if (AllChunks.Contains(Key))
	{
		AChunk* Chunk = AllChunks[Key];
		int32 RelativeX = X - Chunk->GetActorLocation().X;
		int32 RelativeY = Y - Chunk->GetActorLocation().Y;

		RelativeX = ((RelativeX + 50) / BlockSize) + 7;
		RelativeY = ((RelativeY + 50) / BlockSize) + 7;

		return Chunk->GetBlock(RelativeX, RelativeY, Z);
	}

	return EBlockType::None;
}
