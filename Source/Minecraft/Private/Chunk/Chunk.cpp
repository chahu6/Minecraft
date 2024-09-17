#include "Chunk/Chunk.h"
#include "World/WorldSettings.h"
#include "World/Block/Block.h"
#include "World/Runnable/ChunkGeneratorAsyncTask.h"
#include "Chunk/BlockMeshComponent.h"
#include "Chunk/PlantMeshComponent.h"
#include "World/WorldSettings.h"
#include "World/WorldManager.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BlockMeshComponent = CreateDefaultSubobject<UBlockMeshComponent>(TEXT("BlockMeshComponent"));
	BlockMeshComponent->SetupAttachment(RootComponent);
	BlockMeshComponent->bUseAsyncCooking = true;

	PlantMeshComponent = CreateDefaultSubobject<UPlantMeshComponent>(TEXT("PlantMeshComponent"));
	PlantMeshComponent->SetupAttachment(RootComponent);
	PlantMeshComponent->bUseAsyncCooking = true;

	HeightMap.Init(0, WorldSettings::CHUNK_AREA);
	Blocks.Init({}, WorldSettings::CHUNK_VOLUME);
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

void AChunk::TickUpdate()
{

}

void AChunk::UpdateChunk()
{
	SetChunkState(EChunkState::Rebuild);

	BuildMesh();
	AWorldManager::Get()->TaskQueue.Enqueue(this);
}

void AChunk::AddActiveVoxel(const FBlockData& BlockData)
{
	/*if (!ActiveVoxels.Contains(BlockData))
	{
		ActiveVoxels.Add(BlockData);
	}*/
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
	SetChunkState(EChunkState::Rebuild);
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

void AChunk::BuildMesh()
{
	// 方块Mesh
	BlockMeshComponent->BuildMesh(GenerationMethod);

	// 植物Mesh
	PlantMeshComponent->BuildMesh();

	bIsDirty = false;
}

void AChunk::Render()
{
	if (ChunkState != EChunkState::Loaded && ChunkState != EChunkState::Rebuild) return;

	// 方块Mesh
	BlockMeshComponent->Render();

	// 植物Mesh
	PlantMeshComponent->Render();

	SetChunkState(EChunkState::Rendered);

	bIsRendering = true;
}
