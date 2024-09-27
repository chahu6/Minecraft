#include "World/Generator/GreedyMeshGenerator.h"
#include "World/WorldSettings.h"
#include "World/Data/GlobalInfo.h"
#include "Chunk/MeshData.h"
#include "World/Block/Blocks.h"

void GreedyMeshGenerator::BuildGreedyChunkMesh(GlobalInfo& WorldInfo, const FIntPoint& ChunkVoxelPos)
{
	WorldInfo.MeshDataCache[ChunkVoxelPos].Empty();

	FIntPoint ChunkWorldLocation = ChunkVoxelPos * WorldSettings::CHUNK_SIZE;

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
		case 0: // XÖá
		{
			MainAxisLimit = WorldSettings::CHUNK_SIZE;
			Axis1Limit = WorldSettings::CHUNK_SIZE;
			Axis2Limit = WorldSettings::CHUNK_HEIGHT;
			break;
		}
		case 1: // YÖá
		{
			MainAxisLimit = WorldSettings::CHUNK_SIZE;
			Axis1Limit = WorldSettings::CHUNK_HEIGHT;
			Axis2Limit = WorldSettings::CHUNK_SIZE;
			break;
		}
		case 2: // ZÖá
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
					const FBlockState CurrentBlockState = WorldInfo.GetBlockState(ChunkItr + FIntVector(ChunkWorldLocation.X, ChunkWorldLocation.Y, 0));
					const FBlockState CompareBlockState = WorldInfo.GetBlockState((ChunkItr + AxisMask) + FIntVector(ChunkWorldLocation.X, ChunkWorldLocation.Y, 0));

					const int32 CurrentBlockID = CurrentBlockState.BlockID;
					const int32 CompareBlockID = CompareBlockState.BlockID;

					const bool bCurrentBlockOpaque = !CurrentBlockState.IsAir() && CurrentBlockID != UBlocks::Tallgrass->BlockID && CurrentBlockID != UBlocks::Rose->BlockID;
					const bool bCompareBlockOpaque = !CompareBlockState.IsAir() && CompareBlockID != UBlocks::Tallgrass->BlockID && CompareBlockID != UBlocks::Rose->BlockID;

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
							Mask[N++] = FMask{ (EBlockID)CurrentBlockID, 1 };
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
							Mask[N++] = FMask{ (EBlockID)CompareBlockID, -1 };
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
							WorldInfo.MeshDataCache[ChunkVoxelPos]
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

bool GreedyMeshGenerator::CompareMask(const FMask& M1, const FMask& M2)
{
	return M1.BlockID == M2.BlockID && M1.Normal == M2.Normal;
}

void GreedyMeshGenerator::CreateQuad(const FMask& Mask, const FIntVector& AxisMask, const int32 Width, const int32 Height, const FIntVector& V1, const FIntVector& V2, const FIntVector& V3, const FIntVector& V4, TMap<int32, TSharedPtr<FMeshData>>& MeshDatas)
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
