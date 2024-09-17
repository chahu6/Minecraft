// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk/PlantMeshComponent.h"
#include "Chunk/Chunk.h"
#include "Chunk/MeshData.h"
#include "World/Block/Block.h"
#include "Utils/MinecraftAssetLibrary.h"
#include "World/WorldSettings.h"

UPlantMeshComponent::UPlantMeshComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false; // False

	//  «∑ÒÕ∂…‰“ı”∞
	SetCastShadow(false);
}

void UPlantMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	Chunk = Chunk == nullptr ? Cast<AChunk>(GetOwner()) : Chunk;
}

void UPlantMeshComponent::Render()
{
	FBlockMeta BlockMeta;
	CreateMeshSection_LinearColor(0, MeshData->Vertices, MeshData->Triangles, MeshData->Normals, MeshData->UV0, MeshData->VertexColors, MeshData->Tangents, false);
	if (UMinecraftAssetLibrary::GetBlockMeta(6, BlockMeta))
	{
		SetMaterial(0, BlockMeta.Material);
	}
}

void UPlantMeshComponent::Render(const TMap<int32, TSharedPtr<FMeshData>>& NewMeshDatas)
{
	TSharedPtr<FMeshData> NewMeshData =  NewMeshDatas[6];

	FBlockMeta BlockMeta;
	CreateMeshSection_LinearColor(0, NewMeshData->Vertices, NewMeshData->Triangles, NewMeshData->Normals, NewMeshData->UV0, NewMeshData->VertexColors, NewMeshData->Tangents, false);
	if (UMinecraftAssetLibrary::GetBlockMeta(6, BlockMeta))
	{
		SetMaterial(0, BlockMeta.Material);
	}
}

void UPlantMeshComponent::BuildMesh()
{
	MeshData = MakeShared<FMeshData>();

	for (int32 Z = 0; Z < WorldSettings::CHUNK_HEIGHT; ++Z)
	{
		for (int32 Y = 0; Y < WorldSettings::CHUNK_SIZE; ++Y)
		{
			for (int32 X = 0; X < WorldSettings::CHUNK_SIZE; ++X)
			{
				FBlockData BlockData = Chunk->GetBlock(X, Y, Z);
				if (BlockData.IsPlant())
				{
					int32 Index = MeshData->Vertices.Num();

					MeshData->Vertices.Append({
						FVector(X, Y, Z) * WorldSettings::BlockSize,
						FVector(X + 1, Y + 1, Z) * WorldSettings::BlockSize,
						FVector(X, Y, Z + 1) * WorldSettings::BlockSize,
						FVector(X + 1, Y + 1, Z + 1) * WorldSettings::BlockSize,

						FVector(X + 1, Y, Z) * WorldSettings::BlockSize,
						FVector(X, Y + 1, Z) * WorldSettings::BlockSize,
						FVector(X + 1, Y, Z + 1) * WorldSettings::BlockSize,
						FVector(X, Y + 1, Z + 1) * WorldSettings::BlockSize
					});
					MeshData->Triangles.Append({
						Index,
						Index + 1,
						Index + 2,
						Index + 1,
						Index + 3,
						Index + 2,

						Index + 4,
						Index + 6,
						Index + 5,
						Index + 5,
						Index + 6,
						Index + 7
					});
					MeshData->UV0.Append({
						{ 1.f, 1.f },
						{ 0.f, 1.f },
						{ 1.f, 0.f },
						{ 0.f, 0.f },

						{ 1.f, 1.f },
						{ 0.f, 1.f },
						{ 1.f, 0.f },
						{ 0.f, 0.f }
					});
				}
			}
		}
	}
}
