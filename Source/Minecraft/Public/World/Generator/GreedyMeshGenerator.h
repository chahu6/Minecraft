#pragma once

#include "World/Block/Data/BlockID.h"

class GlobalInfo;
struct FMeshData;

struct FMask
{
	EBlockID BlockID = EBlockID::Air;
	int8 Normal = 0;
};

class GreedyMeshGenerator
{
public:
	static void BuildGreedyChunkMesh(GlobalInfo& WorldInfo, const FIntPoint& ChunkVoxelPos);

private:
	static bool CompareMask(const FMask& M1, const FMask& M2);

	static void CreateQuad(const FMask& Mask, const FIntVector& AxisMask, const int32 Width, const int32 Height, const FIntVector& V1, const FIntVector& V2, const FIntVector& V3, const FIntVector& V4, TMap<int32, TSharedPtr<FMeshData>>& MeshDatas);

private:
	GreedyMeshGenerator() = delete;
	~GreedyMeshGenerator() = delete;
};