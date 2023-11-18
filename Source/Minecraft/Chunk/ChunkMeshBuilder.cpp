#include "ChunkMeshBuilder.h"
#include "Minecraft/World/WorldSettings.h"
#include "Chunk.h"
#include "Kismet/KismetMathLibrary.h"
#include "Minecraft/MinecraftType/FaceType.h"
#include "Minecraft/World/WorldManager.h"
#include "Minecraft/Utils/Utils.h"

int32 GetChunkIndexFromWorld(const FVector& WorldLocation)
{
	int32 ChunkIndex_X = FMath::Floor(WorldLocation.X / ChunkSize);
	int32 ChunkIndex_Y = FMath::Floor(WorldLocation.Y / ChunkSize);
	int32 ChunkIndex_Z = FMath::Floor(WorldLocation.Z / ChunkSize);
	if (!(Utils::OutOfBounds(ChunkIndex_X, 0, WORLD_W) && Utils::OutOfBounds(ChunkIndex_Y, 0, WORLD_D) && Utils::OutOfBounds(ChunkIndex_Z, 0, WORLD_H)))
	{
		return -1;
	}
	
	int32 ChunkIndex = ChunkIndex_X + ChunkIndex_Y * WORLD_W + ChunkIndex_Z * WORLD_AREA;
	return ChunkIndex;
}

bool IsVoid(int32 X, int32 Y, int32 Z, const FVector& WorldLocation, AWorldManager* WorldManager)
{
	int32 Index = GetChunkIndexFromWorld(WorldLocation);
	if (Index == -1)
		return false;

	AChunk* Chunk = WorldManager->GetChunk(Index);


	int32 Local_X = (X + CHUNK_SIZE) % CHUNK_SIZE;
	int32 Local_Y = (Y + CHUNK_SIZE) % CHUNK_SIZE;
	int32 Local_Z = (Z + CHUNK_SIZE) % CHUNK_SIZE;
	if (Chunk->GetBlock(Local_X, Local_Y, Local_Z) > 0)
		return false;
	
	return true;
}

void FChunkMeshBuilder::BuildChunkMesh(AChunk* Chunk, FMeshData& OutMeshData)
{
	int32 Index = OutMeshData.Vertices.Num();

	TArray<FVector> Vertices_Tmp;
	Vertices_Tmp.SetNum(4);

	TArray<int32> Triangles_Tmp;
	Triangles_Tmp.SetNum(6);

	TArray<FLinearColor> VertexColors_Tmp;
	VertexColors_Tmp.SetNum(4);

	FVector ChunkLocation = Chunk->GetActorLocation();

	AWorldManager* WorldManager = Cast<AWorldManager>(Chunk->GetOwner());

	for (int32 X = 0; X < CHUNK_SIZE; ++X)
	{
		for (int32 Y = 0; Y < CHUNK_SIZE; ++Y)
		{
			for (int32 Z = 0; Z < CHUNK_SIZE; ++Z)
			{
				uint8 BlockID = Chunk->GetBlock(X, Y, Z);

				if (BlockID == 0)
					continue;

				// Voxel World Position
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
					OutMeshData.Vertices.Append(Vertices_Tmp);
					
					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 2;
					Triangles_Tmp[3] = Index + 2;
					Triangles_Tmp[4] = Index + 3;
					Triangles_Tmp[5] = Index + 0;
					OutMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Up), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					OutMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					OutMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

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
					OutMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 2;
					Triangles_Tmp[2] = Index + 1;
					Triangles_Tmp[3] = Index + 3;
					Triangles_Tmp[4] = Index + 2;
					Triangles_Tmp[5] = Index + 0;
					OutMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Down), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					OutMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					OutMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

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
					OutMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 3;
					Triangles_Tmp[3] = Index + 1;
					Triangles_Tmp[4] = Index + 2;
					Triangles_Tmp[5] = Index + 3;
					OutMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Left), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					OutMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) ,FVector2D(0.0, 1.0) };
					OutMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

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
					OutMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 2;
					Triangles_Tmp[3] = Index + 2;
					Triangles_Tmp[4] = Index + 3;
					Triangles_Tmp[5] = Index + 0;
					OutMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Right), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					OutMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					OutMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

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
					OutMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 3;
					Triangles_Tmp[3] = Index + 1;
					Triangles_Tmp[4] = Index + 2;
					Triangles_Tmp[5] = Index + 3;
					OutMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::Forward), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					OutMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(1.0, 1.0), FVector2D(1.0, 0.0), FVector2D(0.0, 0.0), FVector2D(0.0, 1.0) };
					OutMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

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
					OutMeshData.Vertices.Append(Vertices_Tmp);

					// 索引
					Triangles_Tmp[0] = Index + 0;
					Triangles_Tmp[1] = Index + 1;
					Triangles_Tmp[2] = Index + 2;
					Triangles_Tmp[3] = Index + 2;
					Triangles_Tmp[4] = Index + 3;
					Triangles_Tmp[5] = Index + 0;
					OutMeshData.Triangles.Append(Triangles_Tmp);

					// 顶点颜色
					float Direction = UKismetMathLibrary::MapRangeClamped(StaticCast<float>(EFaceType::BackGround), 0.0, 5.0, 0.0, 1.0);
					VertexColors_Tmp[0] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[1] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[2] = { 0.0f, 0.0f, 0.0f, Direction };
					VertexColors_Tmp[3] = { 0.0f, 0.0f, 0.0f, Direction };
					OutMeshData.VertexColors.Append(VertexColors_Tmp);

					// UV
					const FVector2D bUVs[] = { FVector2D(0.0, 1.0),FVector2D(1.0, 1.0) ,FVector2D(1.0, 0.0) ,FVector2D(0.0, 0.0) };
					OutMeshData.UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));

					Index += 4;
				}
			}
		}
	}
}
