#pragma once

class GlobalInfo;

class MINECRAFT_API CaveGenerator
{
public:
	static void GeneratorCave(GlobalInfo& WorldInfo, const FIntPoint& ChunkVoxelPos);

private:
	CaveGenerator() = delete;
};