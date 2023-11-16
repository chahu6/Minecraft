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
	if (MeshData.Vertices.IsEmpty()) return;

	ProduralMesh->ClearAllMeshSections();
	ProduralMesh->CreateMeshSection_LinearColor(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.VertexColors, MeshData.Tangents, true);
	FBlockInfoTableRow* BlockInfo = GetBlockInfo(2);
	if (BlockInfo)
		ProduralMesh->SetMaterial(0, BlockInfo->Material);
}

void UChunkMeshComponent::BuildMesh()
{
	FChunkMeshBuilder::BuildChunkMesh(Chunk, MeshData);
}

FBlockInfoTableRow* UChunkMeshComponent::GetBlockInfo(uint8 BlockID)
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Datas/DataTable/DT_BlockDataTable.DT_BlockDataTable'"));
	return DataTable->FindRow<FBlockInfoTableRow>(FName(FString::FromInt(BlockID)), nullptr);
}
