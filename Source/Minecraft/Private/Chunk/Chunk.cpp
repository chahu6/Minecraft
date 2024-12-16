#include "Chunk/Chunk.h"
#include "Chunk/BlockMeshComponent.h"
#include "Chunk/PlantMeshComponent.h"
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
}

void AChunk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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

void AChunk::RenderTerrainMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas)
{
	BlockMeshComponent->Render(MeshDatas);
}

void AChunk::RenderPlantMesh(const TMap<int32, TSharedPtr<FMeshData>>& MeshDatas)
{
	PlantMeshComponent->Render(MeshDatas);
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
