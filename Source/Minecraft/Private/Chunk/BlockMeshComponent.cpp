#include "Chunk/BlockMeshComponent.h"
#include "Chunk/MeshData.h"
#include "Chunk/Chunk.h"
#include "World/Block/Block.h"

UBlockMeshComponent::UBlockMeshComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false; // False

	//  «∑ÒÕ∂…‰“ı”∞
	SetCastShadow(true);
}

void UBlockMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	Chunk = Chunk == nullptr ? Cast<AChunk>(GetOwner()) : Chunk;
}

void UBlockMeshComponent::Render(const TMap<FGameplayTag, TSharedPtr<FMeshData>>& NewMeshDatas)
{
	int32 Key = 0;
	ClearAllMeshSections();
	for (auto MeshData = NewMeshDatas.CreateConstIterator(); MeshData; ++MeshData)
	{
		if (MeshData->Value->Vertices.IsEmpty()) continue;

		if (const UBlock* Block = UBlock::GetBlockByID(MeshData->Key))
		{
			if (!Block->IsFullBlock() && Block->bTranslucent)
			{
				CreateMeshSection_LinearColor(Key, MeshData->Value->Vertices, MeshData->Value->Triangles, MeshData->Value->Normals, MeshData->Value->UV0, MeshData->Value->VertexColors, MeshData->Value->Tangents, false);
			}
			else
			{
				CreateMeshSection_LinearColor(Key, MeshData->Value->Vertices, MeshData->Value->Triangles, MeshData->Value->Normals, MeshData->Value->UV0, MeshData->Value->VertexColors, MeshData->Value->Tangents, true);
			}
			SetMaterial(Key++, Block->Material);
		}
	}
}
