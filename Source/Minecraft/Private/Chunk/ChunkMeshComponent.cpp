#include "Chunk/ChunkMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Chunk/ChunkMeshBuilder.h"
#include "Chunk/ChunkSection.h"
#include "Subsystem/MCGameInstanceSubsystem.h"

UChunkMeshComponent::UChunkMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // False

	ChunkSection = Cast<AChunkSection>(GetOwner());
}

void UChunkMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitProduralMeshComponent(GetOwner()->GetRootComponent());

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
	ChunkSection = ChunkSection == nullptr ? Cast<AChunkSection>(GetOwner()) : ChunkSection;
	ensure(ChunkSection != nullptr);

	FChunkMeshBuilder::BuildChunkMesh(ChunkSection, MeshDatas);
}

void UChunkMeshComponent::ClearMeshData()
{
	MeshDatas.Empty();
}

bool UChunkMeshComponent::InitProduralMeshComponent(USceneComponent* Parent)
{
	ProduralMesh = NewObject<UProceduralMeshComponent>(this, TEXT("ProduralMesh"));
	ProduralMesh->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);

	// ÊÇ·ñÍ¶ÉäÒõÓ°
	ProduralMesh->SetCastShadow(false);
	ProduralMesh->RegisterComponent();

	return ProduralMesh->IsRegistered();
}

FBlockInfoTableRow* UChunkMeshComponent::GetBlockInfo(uint8 BlockID)
{
	UMCGameInstanceSubsystem* MCGameInstance = UGameInstance::GetSubsystem<UMCGameInstanceSubsystem>(GetWorld()->GetGameInstance());
	return MCGameInstance->GetBlockDataTable()->FindRow<FBlockInfoTableRow>(FName(FString::FromInt(BlockID)), nullptr);
}
