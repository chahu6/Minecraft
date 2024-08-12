#pragma once

class AChunk;

class MINECRAFT_API CaveGenerator
{
public:
	static void GeneratorCave(AChunk* Chunk);

private:
	CaveGenerator() = delete;
};