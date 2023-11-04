
#include "Chunk.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SimplexNoiseLibrary.h"

#define GetLocationFromIndex(x) ((x) * BlockSize - 50.0f)
#define GetBlocksIndex(x, y, z)				((x) + (y * 16) + (z * 256))

constexpr int32 CHUNK_SIZE		=		16,
				CHUNK_AREA		=		16 * 16,
				CHUNK_VOLUME	=		16 * 16 * 16,
				CHUNK			=		16 * 16 * 256;

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	ProduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProduralMesh"));
	RootComponent = ProduralMesh;
	// 是否投射阴影
	ProduralMesh->SetCastShadow(true);

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Script/Engine.DataTable'/Game/Blueprints/Datas/DataTable/DT_BlockDataTable.DT_BlockDataTable'"));
	if (DataTableAsset.Succeeded())
	{
		DataTable = DataTableAsset.Object;
	}
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
	BuildChunk(0, 32);
}

void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunk::BuildChunk(float rangeMin, float rangeMax, float inFactor)
{
	CollectBlocksType(rangeMin, rangeMax, inFactor);

	GenerateBlock();

	DrawBlock();
}

void AChunk::BuildBlock(int32 X, int32 Y, int32 Z)
{
	for (uint8 EnumIndex = 0; EnumIndex < StaticCast<uint8>(EFaceType::EFT_MAX); ++EnumIndex)
	{
		EFaceType FaceType = StaticCast<EFaceType>(EnumIndex);

		float LocationX = GetLocationFromIndex(X - 7);
		float LocationY = GetLocationFromIndex(Y - 7);
		float LocationZ = GetLocationFromIndex(Z);
		FVector Position(LocationX, LocationY, LocationZ);
		if (IsCreateFaceInDirection(FaceType, X, Y, Z))
		{
			BuildFace(FaceType, Position, StaticCast<uint8>(Blocks[GetBlocksIndex(X, Y, Z)]));
		}
	}
}

void AChunk::BuildFace(EFaceType FaceType, const FVector& Center, uint8 BlockID)
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
	TempMeshData.VertexColors.Add({0.0f, 0.0f, 0.0f, Direction});
	TempMeshData.VertexColors.Add({0.0f, 0.0f, 0.0f, Direction});
	TempMeshData.VertexColors.Add({0.0f, 0.0f, 0.0f, Direction});
	TempMeshData.VertexColors.Add({0.0f, 0.0f, 0.0f, Direction});

	const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
	TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

	MeshDatas.Add(BlockID, TempMeshData);
}

void AChunk::CollectBlocksType(float rangeMin, float rangeMax, float inFactor)
{
	Blocks.SetNumUninitialized(CHUNK);

	for (int32 x = -7; x <= 8; ++x)
	{
		for (int32 y = -7; y <= 8; ++y)
		{
			float LocationX = GetLocationFromIndex(x);
			float LocationY = GetLocationFromIndex(y);

			FVector ActorLocation = GetActorLocation();

			float Noise_X = LocationX + ActorLocation.X;
			float Noise_Y = LocationY + ActorLocation.Y;
			float LocationZ = USimplexNoiseLibrary::SimplexNoiseInRange2D(Noise_X, Noise_Y, rangeMin, rangeMax, inFactor);
			int32 Noise_Z = FMath::Floor(LocationZ);

			int32 XOffset = x + 7;
			int32 YOffset = y + 7;

			for (int32 height = 0; height < 256; ++height)
			{
				uint8 BlockID = CalcBlockID(height, Noise_Z);
				Blocks[GetBlocksIndex(XOffset, YOffset, height)] = StaticCast<EBlockType>(BlockID);
			}
		}
	}
}

void AChunk::GenerateBlock()
{
	for (int32 X = 0; X < 16; ++X)
	{
		for (int32 Y = 0; Y < 16; ++Y)
		{
			for (int32 Z = 0; Z < 256; ++Z)
			{
				if (Blocks[GetBlocksIndex(X, Y, Z)] > EBlockType::Air)
				{
					BuildBlock(X, Y, Z);
				}
			}
		}
	}
}

void AChunk::DrawBlock()
{
	ProduralMesh->ClearAllMeshSections();
	for (auto& Elem : MeshDatas)
	{
		ProduralMesh->CreateMeshSection_LinearColor(Elem.Key, Elem.Value.Vertices, Elem.Value.Triangles, Elem.Value.Normals, Elem.Value.UV0, Elem.Value.VertexColors, Elem.Value.Tangents, true);
		FBlockInfoTableRow* BlockInfo = GetBlockInfo(Elem.Key);
		if(BlockInfo)
			ProduralMesh->SetMaterial(Elem.Key, BlockInfo->Material);
	}
}

bool AChunk::IsCreateFaceInDirection(EFaceType FaceType, int32 X, int32 Y, int32 Z)
{
	switch (FaceType)
	{
	case EFaceType::Up:
		Z += 1;
		break;
	case EFaceType::Down:
		Z -= 1;
		break;
	case EFaceType::Right:
		Y += 1;
		break;
	case EFaceType::Left:
		Y -= 1;
		break;
	case EFaceType::Forward:
		X += 1;
		break;
	case EFaceType::BackGround:
		X -= 1;
		break;
	}

	if (X < 0 || X >= CHUNK_SIZE || Y < 0 || Y >= CHUNK_SIZE || Z < 0 || Z >= 256)
		return true;

	if (Blocks[GetBlocksIndex(X, Y, Z)] == EBlockType::Air)
		return true;

	return false;
}

uint8 AChunk::CalcBlockID(int32 ZValue, int32 MaxZValue)
{
	if (ZValue > MaxZValue)
	{
		return 1;
	}
	else
	{
		if (ZValue == 0)
		{
			return 5;
		}
		if (ZValue > MaxZValue - 3 && ZValue <= MaxZValue)
		{
			return 4;
		}
	}

	return 2;
}

FBlockInfoTableRow* AChunk::GetBlockInfo(uint8 BlockID)
{
	return DataTable->FindRow<FBlockInfoTableRow>(FName(FString::FromInt(BlockID)), nullptr);
}
