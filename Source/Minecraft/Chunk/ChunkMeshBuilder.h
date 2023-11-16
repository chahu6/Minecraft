#pragma once

#include "Minecraft/MinecraftType/MeshData.h"

class AChunk;

class FChunkMeshBuilder
{
public:
	static void BuildChunkMesh(AChunk* Chunk, FMeshData& OutMeshData);
};