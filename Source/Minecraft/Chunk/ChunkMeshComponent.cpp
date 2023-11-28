#include "ChunkMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "ChunkMeshBuilder.h"
#include "ChunkSection.h"

UChunkMeshComponent::UChunkMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // False

	ChunkSection = Cast<AChunkSection>(GetOwner());
}

void UChunkMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ProduralMesh = NewObject<UProceduralMeshComponent>(this, TEXT("ProduralMesh"));
	ProduralMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	// ÊÇ·ñÍ¶ÉäÒõÓ°
	ProduralMesh->SetCastShadow(false);
	ProduralMesh->RegisterComponent();

	ChunkSection = ChunkSection == nullptr ? Cast<AChunkSection>(GetOwner()) : ChunkSection;
}

void UChunkMeshComponent::Render()
{
	for (const auto& MeshData : MeshDatas)
	{
		if (MeshData.Value.Vertices.IsEmpty()) continue;

		//ProduralMesh->ClearAllMeshSections();
		ProduralMesh->CreateMeshSection_LinearColor(MeshData.Key, MeshData.Value.Vertices, MeshData.Value.Triangles, MeshData.Value.Normals, MeshData.Value.UV0, MeshData.Value.VertexColors, MeshData.Value.Tangents, true);
		FBlockInfoTableRow* BlockInfo = GetBlockInfo(MeshData.Key);

		if (BlockInfo)
		{
			ProduralMesh->SetMaterial(MeshData.Key, BlockInfo->Material);
		}
	}
}

void UChunkMeshComponent::BuildMesh()
{
	FChunkMeshBuilder::BuildChunkMesh(ChunkSection, MeshDatas);
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
