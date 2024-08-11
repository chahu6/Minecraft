#include "Chunk/ChunkMeshComponent.h"
#include "Chunk/ChunkMeshBuilder.h"
#include "Chunk/ChunkSection.h"
#include "World/MinecraftSettings.h"
#include "World/Block/Block.h"
#include "Utils/MinecraftAssetLibrary.h"

UChunkMeshComponent::UChunkMeshComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false; // False

	ChunkSection = Cast<AChunkSection>(GetOwner());

	//  «∑ÒÕ∂…‰“ı”∞
	SetCastShadow(false);
}

void UChunkMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	ChunkSection = ChunkSection == nullptr ? Cast<AChunkSection>(GetOwner()) : ChunkSection;
}

void UChunkMeshComponent::Render()
{
	FBlockMeta BlockMeta;
	for (const TPair<int32, FMeshData>& MeshData : MeshDatas)
	{
		if (MeshData.Value.Vertices.IsEmpty()) continue;

		//ProduralMesh->ClearAllMeshSections();
		CreateMeshSection_LinearColor(MeshData.Key, MeshData.Value.Vertices, MeshData.Value.Triangles, MeshData.Value.Normals, MeshData.Value.UV0, MeshData.Value.VertexColors, MeshData.Value.Tangents, true);

		if (UMinecraftAssetLibrary::GetBlockMeta(MeshData.Key, BlockMeta))
		{
			SetMaterial(MeshData.Key, BlockMeta.Material);
		}
	}
}

void UChunkMeshComponent::BuildMesh()
{
	ChunkSection = ChunkSection == nullptr ? Cast<AChunkSection>(GetOwner()) : ChunkSection;
	check(ChunkSection != nullptr);

	FChunkMeshBuilder::BuildChunkMesh(ChunkSection, MeshDatas);
}

void UChunkMeshComponent::ClearMeshData()
{
	MeshDatas.Empty();
}