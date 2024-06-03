#include "Chunk/Chunk.h"
#include "Save/ChunkSaveGame.h"
#include "World/WorldSettings.h"
#include "Generation/TerrainGenerator.h"
#include "Chunk/ChunkSection.h"

#include "World/Runnable/WorldGeneratorAsyncTask.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkSections.SetNum(WORLD_HEIGHT);
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
	Super::Destroyed();

	// ɾ������
	if (WorldGeneratorTask)
	{
		WorldGeneratorTask->EnsureCompletion();
		delete WorldGeneratorTask;
		WorldGeneratorTask = nullptr;
	}

	for (const auto ChunkSection : ChunkSections)
	{
		if (ChunkSection)
		{
			ChunkSection->Destroy();
		}
	}

	ChunkSections.Empty();
}

AChunkSection* AChunk::GetChunkSection(double Voxel_Z)
{
	return ChunkSections.IsValidIndex(Voxel_Z) ? ChunkSections[Voxel_Z] : nullptr;
}

void AChunk::Dirty()
{
	for (const auto ChunkSection : ChunkSections)
	{
		ChunkSection->SetDirty(true);
	}
}

uint8 AChunk::GetBlock(int32 X, int32 Y, int32 Z)
{
	int32 Index = Z / CHUNK_SIZE;
	if (ChunkSections.IsValidIndex(Index))
	{
		return ChunkSections[Index]->GetBlock(X, Y, Z % CHUNK_SIZE);
	}

	return 0;
}

void AChunk::SetBlock(int32 X, int32 Y, int32 Z, uint8 BlockID)
{
	int32 Index = Z / CHUNK_SIZE;
	if (ChunkSections.IsValidIndex(Index))
	{
		ChunkSections[Index]->SetBlock(X, Y, Z % CHUNK_SIZE, BlockID);
	}
}

void AChunk::BuildAndRenderAsync()
{
	WorldGeneratorTask = new FAsyncTask<FWorldGeneratorAsyncTask>(this);
	WorldGeneratorTask->StartBackgroundTask();
}

bool AChunk::IsDone()
{
	if (WorldGeneratorTask)
	{
		return WorldGeneratorTask->IsDone();
	}

	return false;
}

void AChunk::Load(ITerrainGenerator* Generator)
{
	Generator->GenerateChunk(this);

	// ����ÿ��ChunkSection�Ƿ�Ϊ�ջ�Air
	for (const auto ChunkSection : ChunkSections)
	{
		ChunkSection->RecalculateEmpty();
	}
}

void AChunk::Render()
{
	for (const auto ChunkSection : ChunkSections)
	{
		ChunkSection->Render();
	}
}