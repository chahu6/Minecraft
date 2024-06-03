#include "Chunk/ChunkSection.h"
#include "Chunk/ChunkMeshComponent.h"
#include "World/WorldSettings.h"

AChunkSection::AChunkSection()
{
	PrimaryActorTick.bCanEverTick = false;

	Blocks.Init(0, CHUNK_VOLUME);

	ChunkMesh = CreateDefaultSubobject<UChunkMeshComponent>(TEXT("ChunkMesh"));
	RootComponent = ChunkMesh;
}

void AChunkSection::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// GetActorLocation()���������ڹ��캯�����ã���Ϊ���캯����Actor��λ�û�û�и�ֵ
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
	bIsEmpty = !Blocks.ContainsByPredicate([](uint8 Element) {return Element != 0; });
}