#include "Chunk/ChunkMeshComponent.h"
#include "Chunk/ChunkMeshBuilder.h"
#include "Chunk/ChunkSection.h"
#include "World/MinecraftSettings.h"

UChunkMeshComponent::UChunkMeshComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false; // False

	ChunkSection = Cast<AChunkSection>(GetOwner());

	// �Ƿ�Ͷ����Ӱ
	SetCastShadow(false);
}

void UChunkMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	ChunkSection = ChunkSection == nullptr ? Cast<AChunkSection>(GetOwner()) : ChunkSection;
}

void UChunkMeshComponent::Render()
{
	for (const auto& MeshData : MeshDatas)
	{
		if (MeshData.Value.Vertices.IsEmpty()) continue;

		//ProduralMesh->ClearAllMeshSections();
		CreateMeshSection_LinearColor(MeshData.Key, MeshData.Value.Vertices, MeshData.Value.Triangles, MeshData.Value.Normals, MeshData.Value.UV0, MeshData.Value.VertexColors, MeshData.Value.Tangents, true);
		FBlockInfoTableRow* BlockInfo = GetBlockInfo(MeshData.Key);

		if (BlockInfo)
		{
			SetMaterial(MeshData.Key, BlockInfo->Material);
		}
	}
}

void UChunkMeshComponent::BuildMesh()
{
	ChunkSection = ChunkSection == nullptr ? Cast<AChunkSection>(GetOwner()) : ChunkSection;
	ensure(ChunkSection != nullptr);

	FChunkMeshBuilder::BuildChunkMesh(ChunkSection, MeshDatas);
}

void UChunkMeshComponent::ClearMeshData()
{
	MeshDatas.Empty();
}

FBlockInfoTableRow* UChunkMeshComponent::GetBlockInfo(uint8 BlockID)
{
	const UMinecraftSettings* Setting = GetDefault<UMinecraftSettings>();
	UDataTable* DataTable = Setting->BlockDataTable.LoadSynchronous();
	check(DataTable != nullptr);

	return DataTable->FindRow<FBlockInfoTableRow>(FName(FString::FromInt(BlockID)), nullptr);
}
