#pragma once

#include "CoreMinimal.h"
#include "World/Data/ChunkData.h"

struct FMeshData;

class MINECRAFT_API GlobalInfo
{
public:
	// 所有可见Chunk的数据
	TMap<FIntPoint, TSharedPtr<FChunkData>> ChunkDataMap;

	// 所有可见Chunk的MeshData
	TMap<FIntPoint, TMap<int32, TSharedPtr<FMeshData>>> MeshDataCache;

public:
	FBlockState GetBlockState(const FIntVector& BlockWorldVoxelLocation);
	//void SetBlockState(const FIntVector& BlockWorldVoxelLocation);
};