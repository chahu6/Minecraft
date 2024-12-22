#pragma once

#include "CoreMinimal.h"
#include "World/Data/BlockState.h"
#include "Chunk/MeshData.h"
#include "World/Biome/BiomeID.h"

struct FBlockPos;

struct MINECRAFT_API FChunkData
{
private:
	TArray<int32> HeightMap;

	TArray<FBlockState> BlockStateMap;

	TArray<EBiomeID> Biomes;

	TArray<TTuple<float, float, float, float, float>> Noises;

public:
	// ���пɼ�Chunk��MeshData
	TMap<int32, TSharedPtr<FMeshData>> MeshDataCache;

	// ����ֲ��Mesh
	TMap<int32, TSharedPtr<FMeshData>> PlantMeshDataCache;

public:
	explicit FChunkData();

	FBlockState GetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ) const;

	bool SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockSate);
	bool SetBlockState(int32 X, int32 Y, int32 Z, const FBlockState& BlockSate);

	bool SetBiome(const FBlockPos& Pos, EBiomeID BiomeID);
	bool SetBiome(int32 X, int32 Y, EBiomeID BiomeID);
	EBiomeID GetBiome(const FBlockPos& Pos);
	EBiomeID GetBiome(int32 X, int32 Y);

	void SetHeight(int32 X, int32 Y, int32 Height);
	void SetHeight(int32 Index, int32 Height);
	int32 GetHeight(int32 Index) const;
	int32 GetHeight(const FBlockPos& Pos) const;
	int32 GetHeight(int32 X, int32 Y) const;

	void GetNoises(const FBlockPos& InBlockPos, TTuple<float, float, float, float, float>& OutNoises);

	void SetNoiseValues(int32 InX, int32 InY, const TTuple<float, float, float, float, float>& InNoises);
};