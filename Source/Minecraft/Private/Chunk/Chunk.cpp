#include "Chunk/Chunk.h"
#include "Chunk/BlockMeshComponent.h"
#include "Chunk/PlantMeshComponent.h"
#include "World/WorldManager.h"
#include "World/Data/BlockState.h"
#include "World/Data/ChunkData.h"

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

	ThreadEvent = FPlatformProcess::GetSynchEventFromPool();
}

void AChunk::SetInUse(bool InUse)
{
	Super::SetInUse(InUse);

	if (InUse)
	{
		LoadChunk();
	}
	else
	{
		UnloadChunk();
	}
}

void AChunk::LoadChunk()
{
	WorldManager = Cast<AWorldManager>(GetOwner());
}

void AChunk::UnloadChunk()
{
	ChunkData = nullptr;
	ChunkPos = FChunkPos();
	ChunkState = EChunkState::None;
	WorldManager = nullptr;

	BlockMeshComponent->ClearAllMeshSections();
	PlantMeshComponent->ClearAllMeshSections();
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
}

void AChunk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ThreadEvent)
	{
		FPlatformProcess::ReturnSynchEventToPool(ThreadEvent);
		ThreadEvent = nullptr;
	}
}

void AChunk::Destroyed()
{
	Super::Destroyed();

	//AbandonWork();
	//WorldManager->WorldInfo.Remove(ChunkPos);
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
	//SetChunkState(EChunkState::Rebuild);

	BuildMesh();
	//AWorldManager::Get()->TaskQueue.Enqueue(this);
}

//void AChunk::AddActiveVoxel(const FBlockData& BlockData)
//{
	/*if (!ActiveVoxels.Contains(BlockData))
	{
		ActiveVoxels.Add(BlockData);
	}*/
//}

void AChunk::RenderTerrainMesh()
{
	BlockMeshComponent->Render(WorldManager->WorldInfo.ChunkDataMap[ChunkPos]->MeshDataCache);
}

void AChunk::RenderTerrainMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas)
{
	BlockMeshComponent->Render(MeshDatas);
}

void AChunk::RenderPlantMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas)
{
	PlantMeshComponent->Render(MeshDatas);
}

void AChunk::SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockState)
{
	//WorldManager->PlaceBlock(BlockWorldVoxelLocation, BlockState);
	ChunkData->SetBlockState(BlockOffsetLocation, BlockState);
}

void AChunk::Dirty()
{
	bIsDirty = true;
}

void AChunk::BuildMesh()
{
	// ·½¿éMesh
	//BlockMeshComponent->BuildMesh(GenerationMethod);

	// Ö²ÎïMesh
	//PlantMeshComponent->BuildMesh();

	bIsDirty = false;
}
