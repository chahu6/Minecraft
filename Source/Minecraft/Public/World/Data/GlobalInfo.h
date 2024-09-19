#pragma once

#include "CoreMinimal.h"
#include "World/Data/ChunkData.h"

struct FMeshData;

class MINECRAFT_API GlobalInfo
{
public:
	// ���пɼ�Chunk������
	TMap<FIntPoint, TSharedPtr<FChunkData>> ChunkDataMap;

	// ���пɼ�Chunk��MeshData
	TMap<FIntPoint, TMap<int32, TSharedPtr<FMeshData>>> MeshDataCache;

public:
	FBlockState GetBlockState(const FIntVector& BlockWorldVoxelLocation);
	//void SetBlockState(const FIntVector& BlockWorldVoxelLocation);
};