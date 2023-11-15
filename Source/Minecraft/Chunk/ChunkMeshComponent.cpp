#include "ChunkMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Minecraft/Utils/Utils.h"
#include "Minecraft/WorldManager/WorldManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Minecraft/MinecraftType/BlockType.h"
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

	// 是否投射阴影
	ProduralMesh->SetCastShadow(true);
	ProduralMesh->RegisterComponent();

	Chunk = Chunk == nullptr ? Cast<AChunk>(GetOwner()) : Chunk;
}

void UChunkMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UChunkMeshComponent::RenderingBlock()
{
	GenerateBlocks();

	DrawBlock();
}

void UChunkMeshComponent::DrawBlock()
{
	ProduralMesh->ClearAllMeshSections();
	for (auto& Elem : MeshDatas)
	{
		ProduralMesh->CreateMeshSection_LinearColor(Elem.Key, Elem.Value.Vertices, Elem.Value.Triangles, Elem.Value.Normals, Elem.Value.UV0, Elem.Value.VertexColors, Elem.Value.Tangents, false);
		FBlockInfoTableRow* BlockInfo = GetBlockInfo(Elem.Key);
		if (BlockInfo)
			ProduralMesh->SetMaterial(Elem.Key, BlockInfo->Material);
	}
}

void UChunkMeshComponent::GenerateBlocks()
{
	auto& Temp = Chunk->Blocks;
	for (int32 X = 0; X < 16; ++X)
	{
		for (int32 Y = 0; Y < 16; ++Y)
		{
			for (int32 Z = 0; Z < 16; ++Z)
			{
				if (Temp[GetBlocksIndex(X, Y, Z)] > EBlockType::Air)
				{
					BuildBlock(X, Y, Z);
				}
			}
		}
	}
}

void UChunkMeshComponent::BuildBlock(int32 X, int32 Y, int32 Z)
{
	for (uint8 EnumIndex = 0; EnumIndex < StaticCast<uint8>(EFaceType::EFT_MAX); ++EnumIndex)
	{
		EFaceType FaceType = StaticCast<EFaceType>(EnumIndex);

		if (IsCreateFaceInDirection(FaceType, X, Y, Z))
		{
			BuildFace(FaceType, { GetLocationFromIndex(X - 7), GetLocationFromIndex(Y - 7), GetLocationFromIndex(Z) }, StaticCast<uint8>(Chunk->Blocks[GetBlocksIndex(X, Y, Z)]));
		}
	}
}

void UChunkMeshComponent::BuildFace(EFaceType FaceType, const FVector& Center, uint8 BlockID)
{

	FMeshData TempMeshData;
	if (MeshDatas.Find(BlockID))
	{
		TempMeshData = MeshDatas[BlockID];
	}

	int32 index = TempMeshData.Vertices.Num();

	FVector FaceCenter;
	FVector Up;
	FVector Right;
	bool Reversed;
	switch (FaceType)
	{
	case EFaceType::Up:
	{
		FaceCenter = FVector(0.0f, 0.0f, 50.0f);
		Up = FVector::ForwardVector;
		Right = FVector::RightVector;
		Reversed = false;
		break;
	}
	case EFaceType::Down:
	{
		FaceCenter = { 0.0f, 0.0f, -50.0f };
		Up = FVector::ForwardVector;
		Right = FVector::RightVector;
		Reversed = true;
		break;
	}
	case EFaceType::Right:
	{
		FaceCenter = { 0.0f, 50.0f, 0.0f };
		Up = FVector::UpVector;
		Right = FVector::ForwardVector;
		Reversed = false;
		break;
	}
	case EFaceType::Left:
	{
		FaceCenter = { 0.0f, -50.0f, 0.0f };
		Up = FVector::UpVector;
		Right = FVector::ForwardVector;
		Reversed = true;
		break;
	}
	case EFaceType::Forward:
	{
		FaceCenter = { 50.0f, 0.0f, 0.0f };
		Up = FVector::UpVector;
		Right = FVector::RightVector;
		Reversed = true;
		break;
	}
	case EFaceType::BackGround:
	{
		FaceCenter = { -50.0f, 0.0f, 0.0f };
		Up = FVector::UpVector;
		Right = FVector::RightVector;
		Reversed = false;
		break;
	}
	}

	FVector CenterPosition = Center + FaceCenter;

	// 顶点
	TempMeshData.Vertices.Add(CenterPosition - Up * 50.0f + Right * 50.0f);
	TempMeshData.Vertices.Add(CenterPosition - Up * 50.0f - Right * 50.0f);
	TempMeshData.Vertices.Add(CenterPosition + Up * 50.0f - Right * 50.0f);
	TempMeshData.Vertices.Add(CenterPosition + Up * 50.0f + Right * 50.0f);

	// 索引
	if (Reversed)
	{
		TempMeshData.Triangles.Add(index + 0);
		TempMeshData.Triangles.Add(index + 1);
		TempMeshData.Triangles.Add(index + 2);
		TempMeshData.Triangles.Add(index + 2);
		TempMeshData.Triangles.Add(index + 3);
		TempMeshData.Triangles.Add(index + 0);
	}
	else
	{
		TempMeshData.Triangles.Add(index + 1);
		TempMeshData.Triangles.Add(index + 0);
		TempMeshData.Triangles.Add(index + 3);
		TempMeshData.Triangles.Add(index + 3);
		TempMeshData.Triangles.Add(index + 2);
		TempMeshData.Triangles.Add(index + 1);
	}

	// 顶点颜色
	float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(FaceType), 0.0, 5.0, 0.0, 1.0);
	TempMeshData.VertexColors.Add({ 0.0f, 0.0f, 0.0f, Direction });
	TempMeshData.VertexColors.Add({ 0.0f, 0.0f, 0.0f, Direction });
	TempMeshData.VertexColors.Add({ 0.0f, 0.0f, 0.0f, Direction });
	TempMeshData.VertexColors.Add({ 0.0f, 0.0f, 0.0f, Direction });

	const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
	TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

	MeshDatas.Add(BlockID, TempMeshData);
}

bool UChunkMeshComponent::IsCreateFaceInDirection(EFaceType FaceType, int32 X, int32 Y, int32 Z)
{
	int32 Temp_X = X;
	int32 Temp_Y = Y;
	int32 Temp_Z = Z;

	switch (FaceType)
	{
	case EFaceType::Up:
		Temp_Z += 1;
		break;
	case EFaceType::Down:
		Temp_Z -= 1;
		break;
	case EFaceType::Right:
		Temp_Y += 1;
		break;
	case EFaceType::Left:
		Temp_Y -= 1;
		break;
	case EFaceType::Forward:
		Temp_X += 1;
		break;
	case EFaceType::BackGround:
		Temp_X -= 1;
		break;
	}

	if (OutOfBound(Temp_Z)) return true;

	if (OutOfBound(Temp_X) || OutOfBound(Temp_Y))
	{
		return IsCreateFaceInWorld(Temp_X, Temp_Y, Temp_Z);
	}

	if (Chunk->Blocks[GetBlocksIndex(Temp_X, Temp_Y, Temp_Z)] == EBlockType::Air)
	{
		return true;
	}

	return false;
}

bool UChunkMeshComponent::IsCreateFaceInWorld(int32 X, int32 Y, int32 Z)
{
	AWorldManager* WorldManager = Cast<AWorldManager>(Chunk->GetOwner());
	if (WorldManager)
	{
		FVector Location = Chunk->GetActorLocation();

		float Temp_X = (X - 7) * BlockSize - 50.0f;
		float Temp_Y = (Y - 7) * BlockSize - 50.0f;

		EBlockType NearBlockType = WorldManager->GetBlockTypeFromPosition(Temp_X + Location.X, Temp_Y + Location.Y, Z);
		if (NearBlockType == EBlockType::Air)
		{
			return true;
		}
	}
	return false;
}

bool UChunkMeshComponent::OutOfBound(int32 X) const noexcept
{
	return X < 0 || X >= CHUNK_SIZE;
}

FBlockInfoTableRow* UChunkMeshComponent::GetBlockInfo(uint8 BlockID)
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Datas/DataTable/DT_BlockDataTable.DT_BlockDataTable'"));
	return DataTable->FindRow<FBlockInfoTableRow>(FName(FString::FromInt(BlockID)), nullptr);
}
