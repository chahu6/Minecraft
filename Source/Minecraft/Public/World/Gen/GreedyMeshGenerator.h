#pragma once

//#include "World/Block/Data/BlockID.h"
#include "World/Data/FaceType.h"

class GlobalInfo;
struct FMeshData;
struct FChunkPos;

struct FMask
{
	int32 BlockID = 0;
	int8 Normal = 0;
};

struct FQuadInfo
{
	FVector Pos = FVector::ZeroVector;
	int32 BlockID = 0;
};

class GreedyMeshGenerator
{
public:
	static void BuildGreedyChunkMesh(GlobalInfo& WorldInfo, const FChunkPos& InChunkPos);

private:
	static bool CompareMask(const FMask& M1, const FMask& M2);

	static void CreateQuad(const FMask& Mask, const FIntVector& AxisMask, const int32 Width, const int32 Height, const FIntVector& V1, const FIntVector& V2, const FIntVector& V3, const FIntVector& V4, TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

	static void CreateBlock(const TArray<FQuadInfo>& TranslucentBlocks, TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

	static void CreatePlant(const TArray<FQuadInfo>& PlantBlocks, TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

	static void BuildFace(EFaceType FaceType, const FVector& InBlockPos, const TSharedRef<FMeshData> MeshData);

private:
	GreedyMeshGenerator() = delete;
	~GreedyMeshGenerator() = delete;
};