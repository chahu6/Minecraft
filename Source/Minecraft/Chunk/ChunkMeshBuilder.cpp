#include "ChunkMeshBuilder.h"
#include "Minecraft/World/WorldSettings.h"
#include "ChunkSection.h"
#include "Kismet/KismetMathLibrary.h"
#include "Minecraft/MinecraftType/FaceType.h"
#include "Minecraft/World/WorldManager.h"

bool IsVoid(int32 X, int32 Y, int32 Z, const FVector& WorldLocation, AWorldManager* WorldManager)
{
	int32 ChunkVoxel_X = FMath::Floor(WorldLocation.X / ChunkSize);
	int32 ChunkVoxel_Y = FMath::Floor(WorldLocation.Y / ChunkSize);
	int32 ChunkVoxel_Z = FMath::Floor(WorldLocation.Z / ChunkSize);

	AChunkSection* ChunkSection = WorldManager->GetChunkSection(FVector(ChunkVoxel_X, ChunkVoxel_Y, ChunkVoxel_Z));
	if (ChunkSection == nullptr) // 边界条件
		return false;

	int32 Local_X = (X + CHUNK_SIZE) % CHUNK_SIZE;
	int32 Local_Y = (Y + CHUNK_SIZE) % CHUNK_SIZE;
	int32 Local_Z = (Z + CHUNK_SIZE) % CHUNK_SIZE;
	if (ChunkSection->GetBlock(Local_X, Local_Y, Local_Z) > 1)
		return false;

	return true;
}

void FChunkMeshBuilder::BuildChunkMesh(const AChunkSection* ChunkSection, TMap<uint8, FMeshData>& OutMeshDatas)
{
	int32 Index = 0;

	TArray<FVector> Vertices_Tmp;
	Vertices_Tmp.SetNum(4);

	TArray<int32> Triangles_Tmp;
	Triangles_Tmp.SetNum(6);

	TArray<FLinearColor> VertexColors_Tmp;
	VertexColors_Tmp.SetNum(4);

	FVector ChunkLocation = ChunkSection->GetActorLocation();

	AWorldManager* WorldManager = Cast<AWorldManager>(ChunkSection->GetOwner()->GetOwner());

	for (int32 X = 0; X < CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
		{
			for (int32 Z = 0; Z < CHUNK_SIZE; ++Z)
			{
				uint8 BlockID = ChunkSection->GetBlock(X, Y, Z);

				if (BlockID <= 1)
					continue;

				FMeshData TempMeshData;

				if(OutMeshDatas.Contains(BlockID))
				{
					TempMeshData = OutMeshDatas[BlockID];
				}

				Index = TempMeshData.Vertices.Num();

				// World Position
				int32 World_X = X * BlockSize + ChunkLocation.X;
				int32 World_Y = Y * BlockSize + ChunkLocation.Y;
				int32 World_Z = Z * BlockSize + ChunkLocation.Z;

				// Top Face
				if (IsVoid(X, Y, Z + 1, FVector(World_X, World_Y, World_Z + BlockSize), WorldManager))
				{
					int32 Location_X = X * BlockSize;
					int32 Location_Y = Y * BlockSize;
					int32 Location_Z = Z * BlockSize;

					// 顶点
					Vertices_Tmp[0] = FVector(Location_X,				Location_Y,				Location_Z + BlockSize);
					Vertices_Tmp[1] = FVector(Location_X,				Location_Y + BlockSize,	Location_Z + BlockSize);
					Vertices_Tmp[2] = FVector(Location_X + BlockSize,	Location_Y + BlockSize,	Location_Z + BlockSize);
					Vertices_Tmp[3] = FVector(Location_X + BlockSize,	Location_Y,				Location_Z + BlockSize);
					TempMeshData.Vertices.Append(Vertices_Tmp);
					
					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 2;
					Triangles_Tmp[3] = Index + 2;
					Triangles_Tmp[4] = Index + 3;
					Triangles_Tmp[5] = Index + 0;
					TempMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Up), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					TempMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Down Face
				if (IsVoid(X, Y, Z - 1, FVector(World_X, World_Y, World_Z - BlockSize), WorldManager))
				{
					int32 Location_X = X * BlockSize;
					int32 Location_Y = Y * BlockSize;
					int32 Location_Z = Z * BlockSize;

					// 顶点
					Vertices_Tmp[0] = FVector(Location_X,			  Location_Y,			  Location_Z);
					Vertices_Tmp[1] = FVector(Location_X,			  Location_Y + BlockSize, Location_Z);
					Vertices_Tmp[2] = FVector(Location_X + BlockSize, Location_Y + BlockSize, Location_Z);
					Vertices_Tmp[3] = FVector(Location_X + BlockSize, Location_Y,			  Location_Z);
					TempMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 2;
					Triangles_Tmp[2] = Index + 1;
					Triangles_Tmp[3] = Index + 3;
					Triangles_Tmp[4] = Index + 2;
					Triangles_Tmp[5] = Index + 0;
					TempMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Down), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					TempMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Left Face
				if (IsVoid(X, Y - 1, Z, FVector(World_X, World_Y - BlockSize, World_Z), WorldManager))
				{
					int32 Location_X = X * BlockSize;
					int32 Location_Y = Y * BlockSize;
					int32 Location_Z = Z * BlockSize;

					// 顶点
					Vertices_Tmp[0] = FVector(Location_X,			  Location_Y,	Location_Z			   );
					Vertices_Tmp[1] = FVector(Location_X,			  Location_Y,	Location_Z + BlockSize );
					Vertices_Tmp[2] = FVector(Location_X + BlockSize, Location_Y,	Location_Z + BlockSize );
					Vertices_Tmp[3] = FVector(Location_X + BlockSize, Location_Y,	Location_Z			   );
					TempMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 3;
					Triangles_Tmp[3] = Index + 1;
					Triangles_Tmp[4] = Index + 2;
					Triangles_Tmp[5] = Index + 3;
					TempMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Left), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					TempMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) ,FVector2D(0.0, 1.0) };
					TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Right Face
				if (IsVoid(X, Y + 1, Z, FVector(World_X, World_Y + BlockSize, World_Z), WorldManager))
				{
					int32 Location_X = X * BlockSize;
					int32 Location_Y = Y * BlockSize;
					int32 Location_Z = Z * BlockSize;

					// 顶点
					Vertices_Tmp[0] = FVector(Location_X,				Location_Y + BlockSize, Location_Z			    );
					Vertices_Tmp[1] = FVector(Location_X + BlockSize,	Location_Y + BlockSize, Location_Z			    );
					Vertices_Tmp[2] = FVector(Location_X + BlockSize,	Location_Y + BlockSize, Location_Z + BlockSize  );
					Vertices_Tmp[3] = FVector(Location_X,				Location_Y + BlockSize, Location_Z + BlockSize  );
					TempMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 2;
					Triangles_Tmp[3] = Index + 2;
					Triangles_Tmp[4] = Index + 3;
					Triangles_Tmp[5] = Index + 0;
					TempMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Right), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					TempMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// Forward Face
				if (IsVoid(X + 1, Y, Z, FVector(World_X + BlockSize, World_Y, World_Z), WorldManager))
				{
					int32 Location_X = X * BlockSize;
					int32 Location_Y = Y * BlockSize;
					int32 Location_Z = Z * BlockSize;

					// 顶点
					Vertices_Tmp[0] = FVector(Location_X + BlockSize, Location_Y,			  Location_Z			 );
					Vertices_Tmp[1] = FVector(Location_X + BlockSize, Location_Y,			  Location_Z + BlockSize );
					Vertices_Tmp[2] = FVector(Location_X + BlockSize, Location_Y + BlockSize, Location_Z + BlockSize );
					Vertices_Tmp[3] = FVector(Location_X + BlockSize, Location_Y + BlockSize, Location_Z			 );
					TempMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 3;
					Triangles_Tmp[3] = Index + 1;
					Triangles_Tmp[4] = Index + 2;
					Triangles_Tmp[5] = Index + 3;
					TempMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Forward), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					TempMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(1.0, 1.0), FVector2D(1.0, 0.0), FVector2D(0.0, 0.0), FVector2D(0.0, 1.0) };
					TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				// BackGround Face
				if (IsVoid(X - 1, Y, Z, FVector(World_X - BlockSize, World_Y, World_Z), WorldManager))
				{
					int32 Location_X = X * BlockSize;
					int32 Location_Y = Y * BlockSize;
					int32 Location_Z = Z * BlockSize;

					// 顶点
					Vertices_Tmp[0] = FVector(Location_X, Location_Y,				Location_Z				);
					Vertices_Tmp[1] = FVector(Location_X, Location_Y + BlockSize,   Location_Z				);
					Vertices_Tmp[2] = FVector(Location_X, Location_Y + BlockSize,   Location_Z + BlockSize  );
					Vertices_Tmp[3] = FVector(Location_X, Location_Y,				Location_Z + BlockSize  );
					TempMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 2;
					Triangles_Tmp[3] = Index + 2;
					Triangles_Tmp[4] = Index + 3;
					Triangles_Tmp[5] = Index + 0;
					TempMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::BackGround), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					TempMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					TempMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}

				OutMeshDatas.Add(BlockID, TempMeshData);
			}
		}
	}
}
