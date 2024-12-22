#include "Chunk/BlockMeshComponent.h"
#include "Chunk/MeshData.h"
#include "Chunk/Chunk.h"
#include "World/Block/Block.h"

UBlockMeshComponent::UBlockMeshComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false; // False

	// 是否投射阴影
	SetCastShadow(false);
}

void UBlockMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	Chunk = Chunk == nullptr ? Cast<AChunk>(GetOwner()) : Chunk;
}

void UBlockMeshComponent::Render(const TMap<int32, TSharedPtr<FMeshData>>& NewMeshDatas)
{
	int32 Key = 0;
	ClearAllMeshSections();
	for (auto MeshData = NewMeshDatas.CreateConstIterator(); MeshData; ++MeshData)
	{
		if (MeshData->Value->Vertices.IsEmpty()) continue;

		CreateMeshSection_LinearColor(Key, MeshData->Value->Vertices, MeshData->Value->Triangles, MeshData->Value->Normals, MeshData->Value->UV0, MeshData->Value->VertexColors, MeshData->Value->Tangents, true);

		if (const UBlock* Block = UBlock::GetBlockById(MeshData->Key))
		{
			SetMaterial(Key++, Block->Material);
		}
	}
}

#if 0
void UBlockMeshComponent::BuildChunkMesh()
{
	int32 Index = 0;

	FVector ChunkLocation = Chunk->GetActorLocation();

	AWorldManager* WorldManager = AWorldManager::Get();

	for (int32 X = 0; X < WorldSettings::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldSettings::CHUNK_SIZE; ++Y)
		{
			for (int32 Z = 0; Z < WorldSettings::CHUNK_HEIGHT; ++Z)
			{
				FBlockData BlockData = Chunk->GetBlock(X, Y, Z);

				if (BlockData.ID < EBlockID::Stone)
					continue;

				TSharedPtr<FMeshData> MeshData = nullptr;

				int32 BlockID = static_cast<int32>(BlockData.ID);
				if (MeshDatas.Contains(BlockID))
				{
					MeshData = MeshDatas[BlockID];
				}

				Index = MeshData->Vertices.Num();

				// World Voxel Position
				int32 BlockWorldVoxelLocationX = X + ChunkLocation.X / WorldSettings::BlockSize;
				int32 BlockWorldVoxelLocationY = Y + ChunkLocation.Y / WorldSettings::BlockSize;
				int32 BlockWorldVoxelLocationZ = Z + ChunkLocation.Z / WorldSettings::BlockSize;

				// Top Face
				if (IsVoid(FIntVector(BlockWorldVoxelLocationX, BlockWorldVoxelLocationY, BlockWorldVoxelLocationZ + 1), WorldManager))
				{
					int32 Location_X = X * WorldSettings::BlockSize;
					int32 Location_Y = Y * WorldSettings::BlockSize;
					int32 Location_Z = Z * WorldSettings::BlockSize;

					// 顶点
					MeshData->Vertices.Append({
						FVector(Location_X, Location_Y, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X, Location_Y + WorldSettings::BlockSize, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y + WorldSettings::BlockSize, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y, Location_Z + WorldSettings::BlockSize)
					});

					// 索引
					MeshData->Triangles.Append({
						Index + 0,
						Index + 1,
						Index + 2,
						Index + 2,
						Index + 3,
						Index + 0
					});

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Up), 0.0, 5.0, 0.0, 1.0);
					MeshData->VertexColors.Append({
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction }
					});

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					MeshData->UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Down Face
				if (IsVoid(FIntVector(BlockWorldVoxelLocationX, BlockWorldVoxelLocationY, BlockWorldVoxelLocationZ - 1), WorldManager))
				{
					int32 Location_X = X * WorldSettings::BlockSize;
					int32 Location_Y = Y * WorldSettings::BlockSize;
					int32 Location_Z = Z * WorldSettings::BlockSize;

					// 顶点
					MeshData->Vertices.Append({
						FVector(Location_X, Location_Y, Location_Z),
						FVector(Location_X, Location_Y + WorldSettings::BlockSize, Location_Z),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y + WorldSettings::BlockSize, Location_Z),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y, Location_Z)
					});

					// 索引
					MeshData->Triangles.Append({
						Index + 0,
						Index + 2,
						Index + 1,
						Index + 3,
						Index + 2,
						Index + 0
					});

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Down), 0.0, 5.0, 0.0, 1.0);
					MeshData->VertexColors.Append({
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction }
					});

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					MeshData->UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Left Face
				if (IsVoid(FIntVector(BlockWorldVoxelLocationX, BlockWorldVoxelLocationY - 1, BlockWorldVoxelLocationZ), WorldManager))
				{
					int32 Location_X = X * WorldSettings::BlockSize;
					int32 Location_Y = Y * WorldSettings::BlockSize;
					int32 Location_Z = Z * WorldSettings::BlockSize;

					// 顶点
					MeshData->Vertices.Append({
						FVector(Location_X, Location_Y, Location_Z),
						FVector(Location_X, Location_Y, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y, Location_Z)
					});

					// 索引
					MeshData->Triangles.Append({
						Index + 0,
						Index + 1,
						Index + 3,
						Index + 1,
						Index + 2,
						Index + 3
					});

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Left), 0.0, 5.0, 0.0, 1.0);
					MeshData->VertexColors.Append({
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction }
					});

					// UV
					const FVector2D bUVs[] = { FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) ,FVector2D(0.0, 1.0) };
					MeshData->UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Right Face
				if (IsVoid(FIntVector(BlockWorldVoxelLocationX, BlockWorldVoxelLocationY + 1, BlockWorldVoxelLocationZ), WorldManager))
				{
					int32 Location_X = X * WorldSettings::BlockSize;
					int32 Location_Y = Y * WorldSettings::BlockSize;
					int32 Location_Z = Z * WorldSettings::BlockSize;

					// 顶点
					MeshData->Vertices.Append({
						FVector(Location_X, Location_Y + WorldSettings::BlockSize, Location_Z),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y + WorldSettings::BlockSize, Location_Z),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y + WorldSettings::BlockSize, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X, Location_Y + WorldSettings::BlockSize, Location_Z + WorldSettings::BlockSize)
					});

					// 索引
					MeshData->Triangles.Append({
						Index + 0,
						Index + 1,
						Index + 2,
						Index + 2,
						Index + 3,
						Index + 0
					});

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Right), 0.0, 5.0, 0.0, 1.0);

					MeshData->VertexColors.Append({
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction }
					});

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					MeshData->UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Forward Face
				if (IsVoid(FIntVector(BlockWorldVoxelLocationX + 1, BlockWorldVoxelLocationY, BlockWorldVoxelLocationZ), WorldManager))
				{
					int32 Location_X = X * WorldSettings::BlockSize;
					int32 Location_Y = Y * WorldSettings::BlockSize;
					int32 Location_Z = Z * WorldSettings::BlockSize;

					// 顶点
					MeshData->Vertices.Append({
						FVector(Location_X + WorldSettings::BlockSize, Location_Y, Location_Z),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y + WorldSettings::BlockSize, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X + WorldSettings::BlockSize, Location_Y + WorldSettings::BlockSize, Location_Z)
					});

					// 索引
					MeshData->Triangles.Append({
						Index + 0,
						Index + 1,
						Index + 3,
						Index + 1,
						Index + 2,
						Index + 3
					});

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Forward), 0.0, 5.0, 0.0, 1.0);
					MeshData->VertexColors.Append({
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
					});

					// UV
					const FVector2D bUVs[] = { FVector2D(1.0, 1.0), FVector2D(1.0, 0.0), FVector2D(0.0, 0.0), FVector2D(0.0, 1.0) };
					MeshData->UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// BackGround Face
				if (IsVoid(FIntVector(BlockWorldVoxelLocationX - 1, BlockWorldVoxelLocationY, BlockWorldVoxelLocationZ), WorldManager))
				{
					int32 Location_X = X * WorldSettings::BlockSize;
					int32 Location_Y = Y * WorldSettings::BlockSize;
					int32 Location_Z = Z * WorldSettings::BlockSize;

					// 顶点
					MeshData->Vertices.Append({
						FVector(Location_X, Location_Y, Location_Z),
						FVector(Location_X, Location_Y + WorldSettings::BlockSize, Location_Z),
						FVector(Location_X, Location_Y + WorldSettings::BlockSize, Location_Z + WorldSettings::BlockSize),
						FVector(Location_X, Location_Y, Location_Z + WorldSettings::BlockSize)
					});

					// 索引
					MeshData->Triangles.Append({
						Index + 0,
						Index + 1,
						Index + 2,
						Index + 2,
						Index + 3,
						Index + 0
					});

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Background), 0.0, 5.0, 0.0, 1.0);
					MeshData->VertexColors.Append({
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
						{ 0.0f, 0.0f, 0.0f, Direction },
					});

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					MeshData->UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				check(BlockID != 0);
				MeshDatas.Add(BlockID, MeshData);
			}
		}
	}
}

bool UBlockMeshComponent::IsVoid(const FIntVector& BlockWorldVoxelLocation, AWorldManager* WorldManager)
{
	if (WorldManager->GetBlock(BlockWorldVoxelLocation).IsValid())
	{
		return false;
	}

	return true;
}
#endif