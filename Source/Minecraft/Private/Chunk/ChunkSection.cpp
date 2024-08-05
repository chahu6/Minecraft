#include "Chunk/ChunkSection.h"
#include "Chunk/ChunkMeshComponent.h"
#include "World/WorldSettings.h"
#include "World/Block/Block.h"

AChunkSection::AChunkSection()
{
	PrimaryActorTick.bCanEverTick = false;

	Blocks.Init({}, CHUNK_VOLUME);

	ChunkMesh = CreateDefaultSubobject<UChunkMeshComponent>(TEXT("ChunkMesh"));
	RootComponent = ChunkMesh;
}

void AChunkSection::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// GetActorLocation()���������ڹ��캯�����ã���Ϊ���캯����Actor��λ�û�û�и�ֵ
	Center = GetActorLocation() + ChunkSize_Half;
}

FBlockData AChunkSection::GetBlock(int32 X, int32 Y, int32 Z) const
{
	return Blocks[GetBlocksIndex(X, Y, Z)];
}

FBlockData AChunkSection::GetBlock(int32 Index) const
{
	return Blocks[Index];
}

void AChunkSection::SetBlock(int32 X, int32 Y, int32 Z, const FBlockData& BlockData)
{
	Blocks[GetBlocksIndex(X, Y, Z)] = BlockData;
}

void AChunkSection::SetBlock(int32 Index, const FBlockData& BlockData)
{
	Blocks[Index] = BlockData;
}

void AChunkSection::SetBlock(const FVector& OffsetLocation, const FBlockData& BlockData)
{
	Blocks[GetBlocksIndex(OffsetLocation.X, OffsetLocation.Y, OffsetLocation.Z)] = BlockData;
}

void AChunkSection::BuildMesh()
{
	if (bIsDirty)
	{
		if (!bIsEmpty)
		{
			ChunkMesh->ClearMeshData();

			ChunkMesh->BuildMesh();

			bIsReady = true;
		}
		bIsDirty = false;
	}
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

void AChunkSection::RecalculateEmpty()
{
	bIsEmpty = !Blocks.ContainsByPredicate([](const FBlockData& Element) { return Element.ID != EBlockID::Air; });
}
