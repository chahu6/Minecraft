#pragma once

#include "CoreMinimal.h"
#include "World/Data/BlockState.h"
#include "Chunk/MeshData.h"

struct MINECRAFT_API FChunkData
{
public:
	TArray<int32> HeightMap;

	TArray<FBlockState> BlockStateMap;

	TArray<FBlockState> ActiveVoxels;

	// 所有可见Chunk的MeshData
	TMap<int32, TSharedPtr<FMeshData>> MeshDataCache;

	// 所有植物Mesh
	TMap<int32, TSharedPtr<FMeshData>> PlantMeshDataCache;

public:
	explicit FChunkData();

	FBlockState GetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ) const;

	bool SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockSate);

	bool SetBlockState(int32 OffsetX, int32 OffsetY, int32 OffsetZ, const FBlockState& BlockSate);

	void SetHeight(int32 Index, int32 Height);

	int32 GetHeight(int32 Index) const;

	int32 GetHeight(int32 OffsetX, int32 OffsetY) const;

	void TickUpdate();

};