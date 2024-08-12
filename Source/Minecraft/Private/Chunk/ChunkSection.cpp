#include "Chunk/ChunkSection.h"
#include "Chunk/ChunkMeshComponent.h"
#include "World/WorldSettings.h"
#include "World/Block/Block.h"
#include "Core/BlockPos.h"

AChunkSection::AChunkSection()
{
	PrimaryActorTick.bCanEverTick = false;

	Blocks.Init({}, CHUNK_VOLUME);

	ChunkMesh = CreateDefaultSubobject<UChunkMeshComponent>(TEXT("ChunkMesh"));
	RootComponent = ChunkMesh;
	ChunkMesh->bUseAsyncCooking = true;
}

void AChunkSection::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// GetActorLocation()函数不能在构造函数中用，因为构造函数中Actor的位置还没有赋值
	Center = GetActorLocation() + ChunkSize_Half;
}

FBlockData AChunkSection::GetBlock(int32 OffsetX, int32 OffsetY, int32 OffsetZ) const
{
	return Blocks[GetBlocksIndex(OffsetX, OffsetY, OffsetZ)];
}

FBlockData AChunkSection::GetBlock(const FVector& OffsetLocation) const
{
	return Blocks[GetBlocksIndex(OffsetLocation.X, OffsetLocation.Y, OffsetLocation.Z)];
}

FBlockData AChunkSection::GetBlock(const FBlockPos& BlockPos) const
{
	return GetBlock(BlockPos.OffsetLocation());
}

FBlockData AChunkSection::GetBlock(int32 Index) const
{
	return Blocks[Index];
}

void AChunkSection::SetBlock(int32 OffsetX, int32 OffsetY, int32 OffsetZ, const FBlockData& BlockData)
{
	Blocks[GetBlocksIndex(OffsetX, OffsetY, OffsetZ)] = BlockData;
}

void AChunkSection::SetBlock(int32 Index, const FBlockData& BlockData)
{
	Blocks[Index] = BlockData;
}

void AChunkSection::SetBlock(const FVector& OffsetLocation, const FBlockData& BlockData)
{
	Blocks[GetBlocksIndex(OffsetLocation.X, OffsetLocation.Y, OffsetLocation.Z)] = BlockData;
}

void AChunkSection::Render()
{
	if (bIsReady)
	{
		ChunkMesh->Render();
		bIsReady = false;
	}
}

void AChunkSection::Rebuild()
{
	bIsDirty = true;

	BuildMesh();
	Render();
}

void AChunkSection::BuildMesh()
{
	if (bIsDirty)
	{
		if (!bIsEmpty)
		{
			ChunkMesh->BuildMesh();

			bIsReady = true;
		}
		bIsDirty = false;
	}
}

void AChunkSection::RecalculateEmpty()
{
	bIsEmpty = !Blocks.ContainsByPredicate([](const FBlockData& Element) { return Element.ID != EBlockID::Air; });
}
