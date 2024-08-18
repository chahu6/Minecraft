#pragma once

class AChunk;

class MINECRAFT_API HeightGenerator
{
public:
	static void GenerateHeight(AChunk* Chunk);

private:
	HeightGenerator() = delete;
};