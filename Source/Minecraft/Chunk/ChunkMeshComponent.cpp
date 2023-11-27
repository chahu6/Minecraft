#include "ChunkMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "ChunkMeshBuilder.h"
#include "Chunk.h"

UChunkMeshComponent::UChunkMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Chunk = Cast<AChunk>(GetOwner());
}

void UChunkMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ProduralMesh = NewObject<UProceduralMeshComponent>(this, TEXT("ProduralMesh"));
	ProduralMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	// ÊÇ·ñÍ¶ÉäÒõÓ°
	ProduralMesh->SetCastShadow(false);
	ProduralMesh->RegisterComponent();

	Chunk = Chunk == nullptr ? Cast<AChunk>(GetOwner()) : Chunk;
}

void UChunkMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UChunkMeshComponent::Render()
{
	for (const auto& MeshData : MeshDatas)
	{
		if (MeshData.Value.Vertices.IsEmpty()) return;

		//ProduralMesh->ClearAllMeshSections();
		ProduralMesh->CreateMeshSection_LinearColor(MeshData.Key, MeshData.Value.Vertices, MeshData.Value.Triangles, MeshData.Value.Normals, MeshData.Value.UV0, MeshData.Value.VertexColors, MeshData.Value.Tangents, true);
		FBlockInfoTableRow* BlockInfo = GetBlockInfo(MeshData.Key);
		if (BlockInfo)
			ProduralMesh->SetMaterial(MeshData.Key, BlockInfo->Material);
	}
}

void UChunkMeshComponent::BuildMesh()
{
	FChunkMeshBuilder::BuildChunkMesh(Chunk, MeshDatas);
}

void UChunkMeshComponent::ClearMeshData()
{
	MeshDatas.Empty();
}

FBlockInfoTableRow* UChunkMeshComponent::GetBlockInfo(uint8 BlockID)
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Datas/DataTable/DT_BlockDataTable.DT_BlockDataTable'"));
	return DataTable->FindRow<FBlockInfoTableRow>(FName(FString::FromInt(BlockID)), nullptr);
}
