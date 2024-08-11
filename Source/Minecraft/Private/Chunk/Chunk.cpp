#include "Chunk/Chunk.h"
#include "Save/ChunkSaveGame.h"
#include "World/WorldSettings.h"
#include "Chunk/ChunkSection.h"
#include "World/Block/Block.h"
#include "World/Runnable/ChunkGeneratorAsyncTask.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkSections.SetNum(WORLD_HEIGHT);

	HeightMap.SetNum(256);
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FVector ChunkSectionLocation = GetActorLocation();
		for (int32 Z = 0; Z < WORLD_HEIGHT; ++Z)
		{
			ChunkSectionLocation.Z = Z * ChunkSize;
			AChunkSection* ChunkSection = World->SpawnActor<AChunkSection>(AChunkSection::StaticClass(), ChunkSectionLocation, FRotator::ZeroRotator, SpawnParams);
			ChunkSections[Z] = ChunkSection;
		}
	}
}

void AChunk::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AChunk::Destroyed()
{
	// 删除任务
	if (ChunkGeneratorTask)
	{
		ChunkGeneratorTask->EnsureCompletion();
		delete ChunkGeneratorTask;
		ChunkGeneratorTask = nullptr;
	}

	for (AChunkSection* ChunkSection : ChunkSections)
	{
		if (ChunkSection)
		{
			ChunkSection->Destroy();
		}
	}

	ChunkSections.Empty();

	Super::Destroyed();
}

AChunkSection* AChunk::GetChunkSection(double Voxel_Z)
{
	return ChunkSections.IsValidIndex(Voxel_Z) ? ChunkSections[Voxel_Z] : nullptr;
}

void AChunk::Dirty()
{
	for (AChunkSection* ChunkSection : ChunkSections)
	{
		ChunkSection->SetDirty(true);
	}
}

FBlockData AChunk::GetBlock(int32 X, int32 Y, int32 Z)
{
	int32 Index = Z / CHUNK_SIZE;
	if (ChunkSections.IsValidIndex(Index))
	{
		return ChunkSections[Index]->GetBlock(X, Y, Z % CHUNK_SIZE);
	}

	return {};
}

void AChunk::SetBlock(int32 X, int32 Y, int32 Z, const FBlockData& BlockData)
{
	int32 Index = Z / CHUNK_SIZE;
	if (ChunkSections.IsValidIndex(Index))
	{
		ChunkSections[Index]->SetBlock(X, Y, Z % CHUNK_SIZE, BlockData);
	}
}

void AChunk::BuildAndRender()
{
	for (const auto ChunkSection : ChunkSections)
	{
		ChunkSection->BuildMesh();
	}
	Render();
}

void AChunk::BuildAndRenderAsync()
{
	if (bIsRendering) return;
	ChunkGeneratorTask = new FAsyncTask<FChunkGeneratorAsyncTask>(this);
	ChunkGeneratorTask->StartBackgroundTask();
}

bool AChunk::IsDone()
{
	if (ChunkGeneratorTask)
	{
		return ChunkGeneratorTask->IsDone();
	}

	return false;
}

void AChunk::UpdateBlock()
{
	// 计算每个ChunkSection是否为空或Air
	for (AChunkSection* ChunkSection : ChunkSections)
	{
		ChunkSection->RecalculateEmpty();
	}
}

void AChunk::Render()
{
	for (AChunkSection* ChunkSection : ChunkSections)
	{
		ChunkSection->Render();
	}

	bIsRendering = true;
}
