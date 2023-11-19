#include "Chunk.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/Save/ChunkSaveGame.h"
#include "ChunkMeshComponent.h"
#include "Minecraft/World/WorldSettings.h"
#include "Minecraft/Generation/TerrainGenerator.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	Blocks.Init(0, CHUNK_VOLUME);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkMesh = CreateDefaultSubobject<UChunkMeshComponent>(TEXT("ChunkMesh"));
}

void AChunk::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AChunk::OnConstruction(const FTransform& Transform)
{
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();

}

void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

uint8 AChunk::GetBlock(int32 X, int32 Y, int32 Z)
{
	return Blocks[GetBlocksIndex(X, Y, Z)];
}

uint8 AChunk::GetBlock(int32 Index)
{
	return Blocks[Index];
}

void AChunk::SetBlock(int32 X, int32 Y, int32 Z, uint8 BlockID)
{
	Blocks[GetBlocksIndex(X, Y, Z)] = BlockID;
}

void AChunk::SetBlock(int32 Index, uint8 BlockID)
{
	Blocks[Index] = BlockID;
}

void AChunk::Render()
{
	if (bIsDirty)
	{
		if (!bIsEmpty)
		{
			ChunkMesh->ClearMeshData();

			ChunkMesh->BuildMesh();

			ChunkMesh->Render();
		}
		bIsDirty = false;
	}
}

void AChunk::Load(ITerrainGenerator* Generator)
{
	Generator->GenerateChunk(this);

	for (uint8 Element : Blocks)
	{
		if (Element != 0)
		{
			bIsEmpty = false;
			break;
		}
	}
	// Ҳ������ôд
	//bIsEmpty = Blocks.ContainsByPredicate([](uint8 Element) {return Element != 0; });
}

void AChunk::Rebuild()
{
	bIsDirty = true;

	Render();
}

void AChunk::RecalculateEmpty()
{
	bIsDirty = !Blocks.ContainsByPredicate([](uint8 Element) {return Element != 0; });
}
