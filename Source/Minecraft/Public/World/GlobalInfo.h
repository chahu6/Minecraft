#pragma once

#include "CoreMinimal.h"
#include "World/Data/ChunkData.h"

struct FMeshData;

class MINECRAFT_API GlobalInfo
{
public:
	// ���пɼ�Chunk������
	TMap<FIntPoint, ChunkData> ChunksMap;

	// ���пɼ�Chunk��MeshData
	TMap<FIntPoint, TMap<int32, TSharedPtr<FMeshData>>> MeshDataCache;

public:
	int32 GetBlock(const FIntVector& BlockWorldVoxelLocation);
};