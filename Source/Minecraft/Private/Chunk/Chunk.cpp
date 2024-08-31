#include "Chunk/Chunk.h"
#include "World/WorldSettings.h"
#include "World/Block/Block.h"
#include "World/Runnable/ChunkGeneratorAsyncTask.h"
#include "Chunk/ChunkMeshComponent.h"
#include "World/WorldSettings.h"
#include "World/WorldManager.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	ChunkMeshComponent = CreateDefaultSubobject<UChunkMeshComponent>(TEXT("ChunkMeshComponent"));
	RootComponent = ChunkMeshComponent;
	ChunkMeshComponent->bUseAsyncCooking = true;

	HeightMap.Init(0, CHUNK_AREA);

	Blocks.Init({}, CHUNK_VOLUME);
}

void AChunk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EnsureCompletion();

	Super::EndPlay(EndPlayReason);
}

void AChunk::EnsureCompletion()
{
	if (ChunkGeneratorTask)
	{
		ChunkGeneratorTask->TryAbandonTask();
		ChunkGeneratorTask->EnsureCompletion();
		delete ChunkGeneratorTask;
		ChunkGeneratorTask = nullptr;
	}
}

void AChunk::StopBuildMesh()
{
	bIsStopped = true;
}

void AChunk::Dirty()
{
	bIsDirty = true;
}

FBlockData AChunk::GetBlock(int32 OffsetX, int32 OffsetY, int32 WorldZ)
{
	if (Blocks.IsValidIndex(GetBlocksIndex(OffsetX, OffsetY, WorldZ)))
	{
		return Blocks[GetBlocksIndex(OffsetX, OffsetY, WorldZ)];
	}

	return {};
}

void AChunk::SetBlock(int32 OffsetX, int32 OffsetY, int32 WorldZ, const FBlockData& BlockData)
{
	if (Blocks.IsValidIndex(GetBlocksIndex(OffsetX, OffsetY, WorldZ)))
	{
		Blocks[GetBlocksIndex(OffsetX, OffsetY, WorldZ)] = BlockData;
	}
}

void AChunk::BuildAndRender()
{
	BuildMesh();
	Render();
}

void AChunk::Rebuild()
{
	ChunkState = EChunkState::Rebuild;
	if (ChunkGeneratorTask)
	{
		ChunkGeneratorTask->EnsureCompletion();
		delete ChunkGeneratorTask;
		ChunkGeneratorTask = nullptr;
	}
	BuildAndRenderAsync();
}

void AChunk::BuildAndRenderAsync()
{
	if (ChunkState != EChunkState::Loaded && ChunkState != EChunkState::Rebuild) return;

	ChunkGeneratorTask = new FAsyncTask<FChunkGeneratorAsyncTask>(this);
	ChunkGeneratorTask->StartBackgroundTask();
}

void AChunk::RecalculateEmpty()
{
	// 计算每个ChunkSection是否为空或Air
	bIsEmpty = !Blocks.ContainsByPredicate([](const FBlockData& Element) { return Element.ID != EBlockID::Air; });
}

void AChunk::Render()
{
	if (ChunkState != EChunkState::Loaded && ChunkState != EChunkState::Rebuild) return;

	ChunkMeshComponent->Render();
	ChunkState = EChunkState::Rendered;

	bIsRendering = true;
}

void AChunk::BuildMesh()
{
	ChunkMeshComponent->BuildMesh(GenerationMethod);

	bIsDirty = false;
}