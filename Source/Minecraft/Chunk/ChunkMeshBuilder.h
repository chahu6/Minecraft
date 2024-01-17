#pragma once

#include "Minecraft/Types/MeshData.h"

class AChunkSection;

class FChunkMeshBuilder
{
public:
	static void BuildChunkMesh(const AChunkSection* ChunkSection, TMap<uint8, FMeshData>& OutMeshDatas);
};