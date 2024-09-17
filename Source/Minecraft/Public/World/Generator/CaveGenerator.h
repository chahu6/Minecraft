#pragma once

class AChunk;
class GlobalInfo;

class MINECRAFT_API CaveGenerator
{
public:
	static void GeneratorCave(AChunk* Chunk);

	static void GeneratorCave(GlobalInfo& WorldInfo, const FIntPoint& ChunkVoxelPos);

private:
	CaveGenerator() = delete;
};