#include "World/Data/ChunkData.h"
#include "Utils/ChunkHelper.h"
#include "World/Block/Block.h"
#include "World/WorldGenerator.h"
#include "Math/BlockPos.h"

FChunkData::FChunkData()
{
	BlockStateMap.Init({}, WorldGenerator::CHUNK_VOLUME);
	HeightMap.Init(0, WorldGenerator::CHUNK_AREA);
	Biomes.Init(EBiomeID::Ocean, WorldGenerator::CHUNK_AREA);
	Noises.Init({}, WorldGenerator::CHUNK_AREA);
}

FBlockState FChunkData::GetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ) const
{
	const int32 Index = FChunkHelper::GetBlockIndex(OffsetX, OffsetY, OffsetZ);

	if (BlockStateMap.IsValidIndex(Index))
	{
		return BlockStateMap[Index];
	}
	return FBlockState();
}

bool FChunkData::SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockSate)
{
	return SetBlockState(BlockOffsetLocation.X, BlockOffsetLocation.Y, BlockOffsetLocation.Z, BlockSate);
}

bool FChunkData::SetBlockState(int32 X, int32 Y, int32 Z, const FBlockState& BlockSate)
{
	const int32 Index = FChunkHelper::GetBlockIndex(X & 15, Y & 15, Z);
	if (BlockStateMap.IsValidIndex(Index))
	{
		BlockStateMap[Index] = BlockSate;
		return true;
	}
	return false;
}

bool FChunkData::SetBiome(const FBlockPos& Pos, EBiomeID BiomeID)
{
	return SetBiome(Pos.X, Pos.Y, BiomeID);
}

bool FChunkData::SetBiome(int32 X, int32 Y, EBiomeID BiomeID)
{
	const int32 Index = FChunkHelper::GetBlockIndex(X & 15, Y & 15);
	if (Biomes.IsValidIndex(Index))
	{
		Biomes[Index] = BiomeID;
	}
	return false;
}

EBiomeID FChunkData::GetBiome(const FBlockPos& Pos)
{
	return GetBiome(Pos.X, Pos.Y);
}

EBiomeID FChunkData::GetBiome(int32 X, int32 Y)
{
	const int32 Index = FChunkHelper::GetBlockIndex(X & 15, Y & 15);
	if (Biomes.IsValidIndex(Index))
	{
		return Biomes[Index];
	}
	check(false);
	return EBiomeID::Ocean;
}

void FChunkData::SetHeight(int32 X, int32 Y, int32 Height)
{
	SetHeight(FChunkHelper::GetBlockIndex(X & 15, Y & 15), Height);
}

void FChunkData::SetHeight(int32 Index, int32 Height)
{
	if (HeightMap.IsValidIndex(Index))
	{
		HeightMap[Index] = Height;
	}
	else
	{
		check(false);
	}
}

int32 FChunkData::GetHeight(int32 Index) const
{
	if (HeightMap.IsValidIndex(Index))
	{
		return HeightMap[Index];
	}
	check(false);
	return 0;
}

int32 FChunkData::GetHeight(const FBlockPos& Pos) const
{
	return GetHeight(Pos.X, Pos.Y);
}

int32 FChunkData::GetHeight(int32 X, int32 Y) const
{
	return GetHeight(FChunkHelper::GetBlockIndex(X & 15, Y & 15));
}

void FChunkData::GetNoises(const FBlockPos& InBlockPos, TTuple<float, float, float, float, float>& OutNoises)
{
	const int32 Index = FChunkHelper::GetBlockIndex(InBlockPos.X & 15, InBlockPos.Y & 15);
	OutNoises = Noises[Index];
}

void FChunkData::SetNoiseValues(int32 InX, int32 InY, const TTuple<float, float, float, float, float>& InNoises)
{
	const int32 Index = FChunkHelper::GetBlockIndex(InX & 15, InY & 15);
	Noises[Index] = InNoises;
}
