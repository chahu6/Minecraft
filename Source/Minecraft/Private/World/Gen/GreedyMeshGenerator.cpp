#include "World/Gen/GreedyMeshGenerator.h"
#include "World/WorldGenerator.h"
#include "World/Data/GlobalInfo.h"
#include "Chunk/MeshData.h"
#include "World/Block/Block.h"
#include "World/Data/ChunkData.h"
#include "Math/ChunkPos.h"
#include "Math/BlockPos.h"
#include "MinecraftGameplayTags.h"

void GreedyMeshGenerator::BuildGreedyChunkMesh(GlobalInfo& WorldInfo, const FChunkPos& InChunkPos)
{
	TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDataCache = WorldInfo.ChunkDataMap[InChunkPos]->MeshDataCache;
	MeshDataCache.Empty();
	const FBlockPos ChunkBlockPos = InChunkPos.ToBlockPos();

	TArray<FQuadInfo> TranslucentBlocks;
	TArray<FQuadInfo> PlantBlocks;

	for (int32 X = 0; X < WorldGenerator::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldGenerator::CHUNK_SIZE; ++Y)
		{
			for (int32 Z = 0; Z < WorldGenerator::CHUNK_HEIGHT; ++Z)
			{
				const FBlockPos BlockPos = ChunkBlockPos + FBlockPos(X, Y, Z);
				const UBlock* Block = WorldInfo.GetBlockState(BlockPos).GetBlock();
				if (Block->BlockID == AIR || Block->GetRenderType() != EBlockRenderType::Surface)
					continue;

				if (Block->bTranslucent && Block->IsFullBlock())
				{
					TranslucentBlocks.Add({ FVector(X, Y, Z), Block->BlockID });
				}
				else if (Block->bTranslucent && !Block->IsFullBlock())
				{
					PlantBlocks.Add({ FVector(X, Y, Z), Block->BlockID });
				}
			}
		}
	}

	// 生成方块面片
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
				MainAxisLimit = WorldGenerator::CHUNK_SIZE;
				Axis1Limit = WorldGenerator::CHUNK_SIZE;
				Axis2Limit = WorldGenerator::CHUNK_HEIGHT;
				break;
			}
			case 1: // Y轴
			{
				MainAxisLimit = WorldGenerator::CHUNK_SIZE;
				Axis1Limit = WorldGenerator::CHUNK_HEIGHT;
				Axis2Limit = WorldGenerator::CHUNK_SIZE;
				break;
			}
			case 2: // Z轴
			{
				MainAxisLimit = WorldGenerator::CHUNK_HEIGHT;
				Axis1Limit = WorldGenerator::CHUNK_SIZE;
				Axis2Limit = WorldGenerator::CHUNK_SIZE;
				break;
			}
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
					const FBlockState CurrentBlockState = WorldInfo.GetBlockState(ChunkItr + FIntVector(ChunkBlockPos.X, ChunkBlockPos.Y, 0));
					const FBlockState CompareBlockState = WorldInfo.GetBlockState((ChunkItr + AxisMask) + FIntVector(ChunkBlockPos.X, ChunkBlockPos.Y, 0));

					const FGameplayTag CurrentBlockID = CurrentBlockState.GetBlock()->BlockID;
					const FGameplayTag CompareBlockID = CompareBlockState.GetBlock()->BlockID;
					const UBlock* CurrentBlock = CurrentBlockState.GetBlock();
					const UBlock* CompareBlock = CompareBlockState.GetBlock();

					const bool bCurrentBlockOpaque = !CurrentBlockState.IsAir() && CurrentBlock->GetRenderType() == EBlockRenderType::Surface && CurrentBlock->IsFullBlock() && !CurrentBlock->bTranslucent;
					const bool bCompareBlockOpaque = !CompareBlockState.IsAir() && CompareBlock->GetRenderType() == EBlockRenderType::Surface && CompareBlock->IsFullBlock() && !CompareBlock->bTranslucent;
					if (bCurrentBlockOpaque == bCompareBlockOpaque)
					{
						Mask[N++] = FMask{ AIR, 0 };
					}
					else if (bCurrentBlockOpaque)
					{
						if (ChunkItr[Axis] == -1)
						{
							Mask[N++] = FMask{ AIR, 0 };
						}
						else
						{
							Mask[N++] = FMask{ CurrentBlockID, 1 };
						}
					}
					else
					{
						if (ChunkItr[Axis] == MainAxisLimit - 1)
						{
							Mask[N++] = FMask{ AIR, 0 };
						}
						else
						{
							Mask[N++] = FMask{ CompareBlockID, -1 };
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
							ChunkItr + DeltaAxis1 + DeltaAxis2,
							MeshDataCache
						);

						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for (int32 l = 0; l < Height; ++l)
						{
							for (int32 k = 0; k < Width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = FMask{ AIR, 0 };
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

	// 生成半透明方块
	CreateBlock(TranslucentBlocks, MeshDataCache);

	// 生成植物面片
	CreatePlant(PlantBlocks, MeshDataCache);
}

bool GreedyMeshGenerator::CompareMask(const FMask& M1, const FMask& M2)
{
	return M1.BlockID == M2.BlockID && M1.Normal == M2.Normal;
}

void GreedyMeshGenerator::CreateQuad(const FMask& Mask, const FIntVector& AxisMask, const int32 Width, const int32 Height, const FIntVector& V1, const FIntVector& V2, const FIntVector& V3, const FIntVector& V4, TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDatas)
{
	if (Mask.BlockID == AIR) return;

	const FVector Normal = FVector(AxisMask * Mask.Normal);
	float Direction = 0.f;

	TSharedPtr<FMeshData> MeshData = nullptr;
	if (MeshDatas.Contains(Mask.BlockID))
	{
		MeshData = MeshDatas[Mask.BlockID];
	}
	else
	{
		MeshData = MeshDatas.Add(Mask.BlockID, MakeShared<FMeshData>());
	}

	const int32 Index = MeshData->Vertices.Num();

	MeshData->Vertices.Append({
		FVector(V1) * WorldGenerator::BlockSize,
		FVector(V2) * WorldGenerator::BlockSize,
		FVector(V3) * WorldGenerator::BlockSize,
		FVector(V4) * WorldGenerator::BlockSize
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

void GreedyMeshGenerator::CreateBlock(const TArray<FQuadInfo>& TranslucentBlocks, TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDatas)
{
	for (const FQuadInfo& BlockInfo : TranslucentBlocks)
	{
		TSharedPtr<FMeshData> MeshData = nullptr;
		if (MeshDatas.Contains(BlockInfo.BlockID))
		{
			MeshData = MeshDatas[BlockInfo.BlockID];
		}
		else
		{
			MeshData = MeshDatas.Add(BlockInfo.BlockID, MakeShared<FMeshData>());
		}

		BuildQuad(BlockInfo.Pos, MeshData.ToSharedRef());
	}
}

void GreedyMeshGenerator::CreatePlant(const TArray<FQuadInfo>& PlantBlocks, TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDatas)
{
	for (const FQuadInfo& PlantInfo : PlantBlocks)
	{
		TSharedPtr<FMeshData> MeshData = nullptr;
		if (MeshDatas.Contains(PlantInfo.BlockID))
		{
			MeshData = MeshDatas[PlantInfo.BlockID];
		}
		else
		{
			MeshData = MeshDatas.Add(PlantInfo.BlockID, MakeShared<FMeshData>());
		}

		int32 Index = MeshData->Vertices.Num();

		const FVector PlantPos = PlantInfo.Pos * WorldGenerator::BlockSize;

		MeshData->Vertices.Append({
			PlantPos,
			PlantPos + FVector(100.f, 100.f, 0.f),
			PlantPos + FVector(100.f, 100.f, 100.f),
			PlantPos + FVector(0.f, 0.f, 100.f),

			PlantPos + FVector(100.f, 0.f, 0.f),
			PlantPos + FVector(0.f, 100.f, 0.f),
			PlantPos + FVector(0.f, 100.f, 100.f),
			PlantPos + FVector(100.f, 0.f, 100.f),
		});

		MeshData->Triangles.Append({
			Index,
			Index + 1,
			Index + 2,
			Index + 0,
			Index + 2,
			Index + 3,

			Index + 4,
			Index + 5,
			Index + 6,
			Index + 4,
			Index + 6,
			Index + 7
		});

		MeshData->UV0.Append({
			{ 0.f, 1.f },
			{ 1.f, 1.f },
			{ 1.f, 0.f },
			{ 0.f, 0.f },

			{ 0.f, 1.f },
			{ 1.f, 1.f },
			{ 1.f, 0.f },
			{ 0.f, 0.f },
		});
	}
}

void GreedyMeshGenerator::BuildQuad(const FVector& Pos, const TSharedRef<FMeshData> MeshData)
{
	BuildFace(EFaceType::Forward, Pos, MeshData);
	BuildFace(EFaceType::Backward, Pos, MeshData);
	BuildFace(EFaceType::Left, Pos, MeshData);
	BuildFace(EFaceType::Right, Pos, MeshData);
	BuildFace(EFaceType::Up, Pos, MeshData);
	BuildFace(EFaceType::Down, Pos, MeshData);
}

bool GreedyMeshGenerator::IsRender(EFaceType FaceType, GlobalInfo& WorldInfo, const FBlockPos& InBlockPos)
{
	FBlockPos Direction;
	switch (FaceType)
	{
		case EFaceType::Up:
			Direction = FBlockPos(0, 0, 1);
			break;
		case EFaceType::Down:
			Direction = FBlockPos(0, 0, -1);
			break;
		case EFaceType::Forward:
			Direction = FBlockPos(1, 0, 0);
			break;
		case EFaceType::Backward:
			Direction = FBlockPos(-1, 0, 0);
			break;
		case EFaceType::Left:
			Direction = FBlockPos(0, -1, 0);
			break;
		case EFaceType::Right:
			Direction = FBlockPos(0, 1, 0);
			break;
	}

	UBlock* Block = WorldInfo.GetBlockState(InBlockPos + Direction).GetBlock();

	return (Block->BlockID == AIR) || Block->bTranslucent || !Block->IsFullBlock();
}

void GreedyMeshGenerator::BuildFace(EFaceType FaceType, const FVector& InBlockPos, const TSharedRef<FMeshData> MeshData)
{
	const int32 Index = MeshData->Vertices.Num();

	FVector FaceCenter;
	FVector Up;
	FVector Right;
	FVector Normal;
	switch (FaceType)
	{
		case EFaceType::Up:
		{
			FaceCenter = InBlockPos + FVector(0.5f, 0.5f, 1.0f);
			Up = FVector::ForwardVector;
			Right = FVector::RightVector;
			Normal = FVector::UpVector;
			break;
		}
		case EFaceType::Down:
		{
			FaceCenter = InBlockPos + FVector(0.5f, 0.5f, 0.0f);
			Up = FVector::BackwardVector;
			Right = FVector::RightVector;
			Normal = FVector::DownVector;
			break;
		}
		case EFaceType::Right:
		{
			FaceCenter = InBlockPos + FVector(0.5f, 1.0f, 0.5f);
			Up = FVector::UpVector;
			Right = FVector::ForwardVector;
			Normal = FVector::RightVector;
			break;
		}
		case EFaceType::Left:
		{
			FaceCenter = InBlockPos + FVector(0.5f, 0.0f, 0.5f);
			Up = FVector::UpVector;
			Right = FVector::BackwardVector;
			Normal = FVector::LeftVector;
			break;
		}
		case EFaceType::Forward:
		{
			FaceCenter = InBlockPos + FVector(1.0f, 0.5f, 0.5f);
			Up = FVector::UpVector;
			Right = FVector::LeftVector;
			Normal = FVector::ForwardVector;
			break;
		}
		case EFaceType::Backward:
		{
			FaceCenter = InBlockPos + FVector(0.0f, 0.5f, 0.5f);
			Up = FVector::UpVector;
			Right = FVector::RightVector;
			Normal = FVector::BackwardVector;
			break;
		}
	}

	FaceCenter *= WorldGenerator::BlockSize;

	MeshData->Vertices.Append({
		FaceCenter - Up * 50.f - Right * 50.f,
		FaceCenter - Up * 50.f + Right * 50.f,
		FaceCenter + Up * 50.f + Right * 50.f,
		FaceCenter + Up * 50.f - Right * 50.f
	});

	MeshData->Triangles.Append({
		Index,
		Index + 1,
		Index + 2,
		Index + 2,
		Index + 3,
		Index + 0,
	});

	MeshData->Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
	});

	MeshData->UV0.Append({
		{ 0.f, 1.f },
		{ 1.f, 1.f },
		{ 1.f, 0.f },
		{ 0.f, 0.f }
	});
}

void GreedyMeshGenerator::BuildChunkMesh(GlobalInfo& WorldInfo, const FChunkPos& InChunkPos)
{
	TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDataCache = WorldInfo.ChunkDataMap[InChunkPos]->MeshDataCache;

	const FBlockPos ChunkBlockPos = InChunkPos.ToBlockPos();

	for (int32 X = 0; X < WorldGenerator::CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < WorldGenerator::CHUNK_SIZE; ++Y)
		{
			for (int32 Z = 0; Z < WorldGenerator::CHUNK_HEIGHT; ++Z)
			{
				const FBlockPos BlockPos = ChunkBlockPos + FBlockPos(X, Y, Z);

				const FBlockState BlockState = WorldInfo.GetBlockState(BlockPos);
				if (BlockState.IsAir() || BlockState.GetBlock()->GetRenderType() != EBlockRenderType::Surface)
					continue;

				TSharedPtr<FMeshData> MeshData = nullptr;
				if (MeshDataCache.Contains(BlockState.GetBlock()->BlockID))
				{
					MeshData = MeshDataCache[BlockState.GetBlock()->BlockID];
				}
				else
				{
					MeshData = MeshDataCache.Add(BlockState.GetBlock()->BlockID, MakeShared<FMeshData>());
				}

				if (IsRender(EFaceType::Up, WorldInfo, BlockPos))
				{
					BuildFace(EFaceType::Up, FVector(X, Y, Z), MeshData.ToSharedRef());
				}
				if (IsRender(EFaceType::Down, WorldInfo, BlockPos))
				{
					BuildFace(EFaceType::Down, FVector(X, Y, Z), MeshData.ToSharedRef());
				}
				if (IsRender(EFaceType::Forward, WorldInfo, BlockPos))
				{
					BuildFace(EFaceType::Forward, FVector(X, Y, Z), MeshData.ToSharedRef());
				}
				if (IsRender(EFaceType::Backward, WorldInfo, BlockPos))
				{
					BuildFace(EFaceType::Backward, FVector(X, Y, Z), MeshData.ToSharedRef());
				}
				if (IsRender(EFaceType::Left, WorldInfo, BlockPos))
				{
					BuildFace(EFaceType::Left, FVector(X, Y, Z), MeshData.ToSharedRef());
				}
				if (IsRender(EFaceType::Right, WorldInfo, BlockPos))
				{
					BuildFace(EFaceType::Right, FVector(X, Y, Z), MeshData.ToSharedRef());
				}
			}
		}
	}
}