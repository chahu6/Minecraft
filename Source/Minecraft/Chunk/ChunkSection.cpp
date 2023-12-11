#include "ChunkSection.h"
#include "ChunkMeshComponent.h"
#include "Minecraft/World/WorldSettings.h"

AChunkSection::AChunkSection()
{
	PrimaryActorTick.bCanEverTick = false;

	Blocks.Init(0, CHUNK_VOLUME);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkMesh = CreateDefaultSubobject<UChunkMeshComponent>(TEXT("ChunkMesh"));
}

void AChunkSection::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// GetActorLocation()函数不能在构造函数中用，因为构造函数中Actor的位置还没有赋值
	Center = GetActorLocation() + ChunkSize_Half;
}

uint8 AChunkSection::GetBlock(int32 X, int32 Y, int32 Z) const
{
	return Blocks[GetBlocksIndex(X, Y, Z)];
}

uint8 AChunkSection::GetBlock(int32 Index) const
{
	return Blocks[Index];
}

void AChunkSection::SetBlock(int32 X, int32 Y, int32 Z, uint8 BlockID)
{
	Blocks[GetBlocksIndex(X, Y, Z)] = BlockID;
}

void AChunkSection::SetBlock(int32 Index, uint8 BlockID)
{
	Blocks[Index] = BlockID;
}

void AChunkSection::SetBlock(const FVector& OffsetLocation, uint8 BlockID)
{
	Blocks[GetBlocksIndex(OffsetLocation.X, OffsetLocation.Y, OffsetLocation.Z)] = BlockID;
}

void AChunkSection::Render()
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

void AChunkSection::Rebuild()
{
	bIsDirty = true;

	Render();
}

void AChunkSection::RecalculateEmpty()
{
	bIsEmpty = !Blocks.ContainsByPredicate([](uint8 Element) {return Element != 0; });
}
