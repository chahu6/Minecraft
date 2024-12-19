#include "Chunk/Chunk.h"
#include "Chunk/BlockMeshComponent.h"
#include "Chunk/PlantMeshComponent.h"
#include "World/WorldManager.h"
#include "World/Data/BlockState.h"
#include "World/Data/ChunkData.h"
#include "World/Runnable/ChunkLoadWork.h"
#include "World/Runnable/ChunkUnloadWork.h"

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
}

void AChunk::SetInUse(bool InUse)
{
	Super::SetInUse(InUse);

	if (!InUse)
	{
		ChunkData = nullptr;
		ChunkPos = FChunkPos();

		BlockMeshComponent->ClearAllMeshSections();
		PlantMeshComponent->ClearAllMeshSections();
	}
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
}

void AChunk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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
	SetChunkState(EChunkState::Rebuild);

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

IQueuedWork* AChunk::MakeLoadWork()
{
	//AbandonWork();
	ThreadWork = new FChunkLoadWork(WorldManager->Get(), ChunkPos);
	return ThreadWork;
}

IQueuedWork* AChunk::MakeUnLoadWork()
{
	//AbandonWork();
	ThreadWork = new FChunkUnloadWork(WorldManager->Get(), ChunkPos);
	return ThreadWork;
}

void AChunk::AbandonWork()
{
	if (ThreadWork != nullptr)
	{
		ThreadWork->Abandon();
		ThreadWork = nullptr;
	}
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
