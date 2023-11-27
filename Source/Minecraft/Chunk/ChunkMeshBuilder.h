#pragma once

#include "Minecraft/MinecraftType/MeshData.h"

class AChunk;

class FChunkMeshBuilder
{
public:
	static void BuildChunkMesh(AChunk* Chunk, TMap<uint8, FMeshData>& OutMeshDatas);
};