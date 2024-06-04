#pragma once

#include "Types/MeshData.h"

class AChunkSection;

struct FMask
{
	uint8 Block;
	int32 Normal;
};

class FChunkMeshBuilder
{
public:
	static void BuildChunkMesh(const AChunkSection* ChunkSection, TMap<uint8, FMeshData>& OutMeshDatas);
	static void BuildGreedyChunkMesh(const AChunkSection* ChunkSection, TMap<uint8, FMeshData>& OutMeshDatas);
};