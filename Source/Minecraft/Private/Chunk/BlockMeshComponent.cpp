#include "Chunk/BlockMeshComponent.h"
#include "Chunk/Chunk.h"
#include "World/MinecraftSettings.h"
#include "World/Block/Block.h"
#include "Utils/MinecraftAssetLibrary.h"
#include "World/WorldManager.h"
#include "World/WorldSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "Chunk/MeshData.h"

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

void UBlockMeshComponent::BuildMesh(EGenerationMethod GenerationMethod)
{
	MeshDatas.Empty();

	switch (GenerationMethod)
	{
	case EGenerationMethod::Normal:
		BuildChunkMesh();
		break;
	case EGenerationMethod::Greedy:
		BuildGreedyChunkMesh();
		break;
	}
}

void UBlockMeshComponent::Render()
{
	FBlockMeta BlockMeta;
	for (auto MeshData = MeshDatas.CreateConstIterator(); MeshData; ++MeshData)
	{
		if (MeshData->Value->Vertices.IsEmpty()) continue;

		CreateMeshSection_LinearColor(MeshData->Key, MeshData->Value->Vertices, MeshData->Value->Triangles, MeshData->Value->Normals, MeshData->Value->UV0, MeshData->Value->VertexColors, MeshData->Value->Tangents, true);

		if (UMinecraftAssetLibrary::GetBlockMeta(MeshData->Key, BlockMeta))
		{
			SetMaterial(MeshData->Key, BlockMeta.Material);
		}
	}
}

void UBlockMeshComponent::Render(const TMap<int32, TSharedPtr<FMeshData>>& NewMeshDatas)
{
	FBlockMeta BlockMeta;
	for (auto MeshData = NewMeshDatas.CreateConstIterator(); MeshData; ++MeshData)
	{
		if (MeshData->Value->Vertices.IsEmpty()) continue;

		CreateMeshSection_LinearColor(MeshData->Key, MeshData->Value->Vertices, MeshData->Value->Triangles, MeshData->Value->Normals, MeshData->Value->UV0, MeshData->Value->VertexColors, MeshData->Value->Tangents, true);

		if (UMinecraftAssetLibrary::GetBlockMeta(MeshData->Key, BlockMeta))
		{
			SetMaterial(MeshData->Key, BlockMeta.Material);
		}
	}
}

void UBlockMeshComponent::BuildGreedyChunkMesh()
{
	AWorldManager* WorldManager = AWorldManager::Get();

	FVector ChunkWorldLocation = Chunk->GetActorLocation();

	for (int32 Axis = 0; Axis < 3; ++Axis)
	{
		const int32 Axis1 = (Axis + 1) % 3;
		const int32 Axis2 = (Axis + 2) % 3;

		int32 MainAxisLimit = 0;
		int32 Axis1Limit = 0;
		int32 Axis2Limit = 0;

		FIntVector DeltaAxis1 = FIntVector::ZeroValue;
		FIntVector DeltaAxis2 = FIntVector::ZeroValue;

		FIntVector ChunkItr = FIntVector::ZeroValue;
		FIntVector AxisMask = FIntVector::ZeroValue;

		AxisMask[Axis] = 1;

		switch (Axis)
		{
			case 0: // X轴
			{
				MainAxisLimit = WorldSettings::CHUNK_SIZE;
				Axis1Limit = WorldSettings::CHUNK_SIZE;
				Axis2Limit = WorldSettings::CHUNK_HEIGHT;
				break;
			}
			case 1: // Y轴
			{
				MainAxisLimit = WorldSettings::CHUNK_SIZE;
				Axis1Limit = WorldSettings::CHUNK_HEIGHT;
				Axis2Limit = WorldSettings::CHUNK_SIZE;
				break;
			}
			case 2: // Z轴
			{
				MainAxisLimit = WorldSettings::CHUNK_HEIGHT;
				Axis1Limit = WorldSettings::CHUNK_SIZE;
				Axis2Limit = WorldSettings::CHUNK_SIZE;
				break;
			}
			default:
				break;
		}

		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit);

		for (ChunkItr[Axis] = -1; ChunkItr[Axis] < MainAxisLimit;)
		{
			int32 N = 0;

			for (ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis2Limit; ++ChunkItr[Axis2])
			{
				for (ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis1Limit; ++ChunkItr[Axis1])
				{
					const FBlockData CurrentBlock = WorldManager->GetBlock(ChunkItr + FIntVector(ChunkWorldLocation / WorldSettings::BlockSize));
					const FBlockData CompareBlock = WorldManager->GetBlock((ChunkItr + AxisMask) + FIntVector(ChunkWorldLocation / WorldSettings::BlockSize));

					const bool bCurrentBlockOpaque = CurrentBlock.ID != EBlockID::Air && !CurrentBlock.IsPlant();
					const bool bCompareBlockOpaque = CompareBlock.ID != EBlockID::Air && !CompareBlock.IsPlant();

					if (bCurrentBlockOpaque == bCompareBlockOpaque)
					{
						Mask[N++] = FMask{ EBlockID::Air, 0 };
					}
					else if (bCurrentBlockOpaque)
					{
						if (ChunkItr[Axis] == -1)
						{
							Mask[N++] = FMask{ EBlockID::Air, 0 };
						}
						else
						{
							Mask[N++] = FMask{ CurrentBlock.ID, 1 };
						}
					}
					else
					{
						if (ChunkItr[Axis] == MainAxisLimit - 1)
						{
							Mask[N++] = FMask{ EBlockID::Air, 0 };
						}
						else
						{
							Mask[N++] = FMask{ CompareBlock.ID, -1 };
						}
					}
				}
			}

			++ChunkItr[Axis];
			N = 0;

			for (int32 j = 0; j < Axis2Limit; ++j)
			{
				for (int32 i = 0; i < Axis1Limit;)
				{
					if (Mask[N].Normal != 0)
					{
						const FMask CurrentMask = Mask[N];
						ChunkItr[Axis1] = i;
						ChunkItr[Axis2] = j;

						int32 Width;
						for (Width = 1; i + Width < Axis1Limit && CompareMask(Mask[N + Width], CurrentMask); ++Width)
						{
						}

						int32 Height;
						bool bDone = false;

						for (Height = 1; j + Height < Axis2Limit; ++Height)
						{
							for (int32 k = 0; k < Width; ++k)
							{
								if (CompareMask(Mask[N + k + Height * Axis1Limit], CurrentMask))
								{
									continue;
								}

								bDone = true;
								break;
							}

							if (bDone) break;
						}
						DeltaAxis1[Axis1] = Width;
						DeltaAxis2[Axis2] = Height;

						CreateQuad(
							CurrentMask, AxisMask, Width, Height,
							ChunkItr,
							ChunkItr + DeltaAxis1,
							ChunkItr + DeltaAxis2,
							ChunkItr + DeltaAxis1 + DeltaAxis2
						);

						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for (int32 l = 0; l < Height; ++l)
						{
							for (int32 k = 0; k < Width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = FMask{ EBlockID::Air, 0 };
							}
						}

						i += Width;
						N += Width;
					}
					else
					{
						i++;
						N++;
					}
				}
			}
		}
	}
}

bool UBlockMeshComponent::CompareMask(const FMask& M1, const FMask& M2)
{
	return M1.BlockID == M2.BlockID && M1.Normal == M2.Normal;
}

void UBlockMeshComponent::CreateQuad(const FMask& Mask, const FIntVector& AxisMask, const int32 Width, const int32 Height, const FIntVector& V1, const FIntVector& V2, const FIntVector& V3, const FIntVector& V4)
{
	if (Mask.BlockID == EBlockID::Air) return;

	const FVector Normal = FVector(AxisMask * Mask.Normal);
	float Direction = 0.f;

	TSharedPtr<FMeshData> MeshData = nullptr;
	if (MeshDatas.Contains(StaticCast<int32>(Mask.BlockID)))
	{
		MeshData = MeshDatas[StaticCast<int32>(Mask.BlockID)];
	}
	else
	{
		MeshData = MeshDatas.Add(StaticCast<int32>(Mask.BlockID), MakeShared<FMeshData>());
	}

	int32 Index = MeshData->Vertices.Num();

	MeshData->Vertices.Append({
		FVector(V1) * WorldSettings::BlockSize,
		FVector(V2) * WorldSettings::BlockSize,
		FVector(V3) * WorldSettings::BlockSize,
		FVector(V4) * WorldSettings::BlockSize
	});

	MeshData->Triangles.Append({
		Index,
		Index + 2 + Mask.Normal,
		Index + 2 - Mask.Normal,
		Index + 3,
		Index + 1 - Mask.Normal,
		Index + 1 + Mask.Normal
	});

	MeshData->Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
	});

	if (Normal.X || Normal.Y)
	{
		Direction = 0.2f;
	}
	else if (Normal.Z < 0)
	{
		Direction = 1.0f;
	}
	else
	{
		Direction = 0.8f;
	}

	MeshData->VertexColors.Append({
		{ 0.0f, 0.0f, 0.0f, Direction },
		{ 0.0f, 0.0f, 0.0f, Direction },
		{ 0.0f, 0.0f, 0.0f, Direction },
		{ 0.0f, 0.0f, 0.0f, Direction }
	});

	if (Normal.X == 1 || Normal.X == -1)
	{
		MeshData->UV0.Append({
			FVector2D(Width, Height),
			FVector2D(0, Height),
			FVector2D(Width, 0),
			FVector2D(0, 0)
		});
	}
	else
	{
		MeshData->UV0.Append({
			FVector2D(Height, Width),
			FVector2D(Height, 0),
			FVector2D(0, Width),
			FVector2D(0, 0),
		});
	}
}

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
