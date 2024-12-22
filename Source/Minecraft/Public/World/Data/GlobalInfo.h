#pragma once

#include "CoreMinimal.h"
#include "Math/ChunkPos.h"

struct FChunkData;
struct FBlockState;

class MINECRAFT_API GlobalInfo
{
public:
	// ���пɼ�Chunk������
	TMap<FChunkPos, TSharedPtr<FChunkData>> ChunkDataMap;

public:
	void Add(const FChunkPos& ChunkPos, TSharedPtr<FChunkData> ChunkData);

	void Remove(const FChunkPos& ChunkPos);

	FBlockState GetBlockState(const FIntVector& BlockWorldVoxelLocation);
	FBlockState GetBlockState(const FBlockPos& InBlockPos);
	FBlockState GetBlockState(int32 X, int32 Y, int32 Z);
	//void SetBlockState(const FIntVector& BlockWorldVoxelLocation);

	FCriticalSection CriticalSection;
};