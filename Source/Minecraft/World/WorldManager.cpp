#include "WorldManager.h"
#include "Minecraft/World/WorldSettings.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/Generation/ClassicOverWorldGenerator.h"
#include "SimplexNoiseLibrary.h"

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;

	Chunks.SetNum(WORLD_VOL);

	TerrainGenerator = MakeUnique<FClassicOverWorldGenerator>();
}

void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	USimplexNoiseLibrary::SetNoiseSeed(234324);
	
	BuildChunks();
	BuildChunkMesh();
	Render();
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldManager::BuildChunks()
{
	for (int32 X = 0; X < WORLD_W; ++X)
	{
		for (int32 Y = 0; Y < WORLD_D; ++Y)
		{
			for (int32 Z = 0; Z < WORLD_H; ++Z)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					AChunk* Chunk = World->SpawnActor<AChunk>(AChunk::StaticClass(), FVector(X * ChunkSize, Y * ChunkSize, Z * ChunkSize), FRotator::ZeroRotator, SpawnParams);
					int32 ChunkIndex = X + WORLD_W * Y + WORLD_AREA * Z;
					Chunks[ChunkIndex] = Chunk;
					Chunk->Load(TerrainGenerator.Get());
				}
			}
		}
	}
}

void AWorldManager::BuildChunkMesh()
{
	for (auto& Chunk : Chunks)
	{
		Chunk->BuildChunkMesh();
	}
}

void AWorldManager::Render()
{
	for (auto Chunk : Chunks)
	{
		Chunk->Render();
	}
}

AChunk* AWorldManager::GetChunk(int32 Index)
{
	return Chunks[Index];
}
