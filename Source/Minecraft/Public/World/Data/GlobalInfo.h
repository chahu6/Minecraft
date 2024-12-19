#pragma once

#include "CoreMinimal.h"
#include "Math/ChunkPos.h"
//#include "World/Data/BlockState.h"

struct FChunkData;
struct FBlockState;

class MINECRAFT_API GlobalInfo
{
public:
	// 所有可见Chunk的数据
	TMap<FChunkPos, TSharedPtr<FChunkData>> ChunkDataMap;

public:

	void Add(const FChunkPos& ChunkPos, TSharedPtr<FChunkData> ChunkData)
	{
		//RWLock.WriteLock();
		CriticalSection.Lock();
		ChunkDataMap.Add(ChunkPos, ChunkData);
		//RWLock.WriteUnlock();
		CriticalSection.Unlock();
	}

	void Remove(const FChunkPos& ChunkPos)
	{
		CriticalSection.Lock();
		ChunkDataMap.Remove(ChunkPos);
		CriticalSection.Unlock();
	}

	FBlockState GetBlockState(const FIntVector& BlockWorldVoxelLocation);
	//void SetBlockState(const FIntVector& BlockWorldVoxelLocation);

	//FRWLock RWLock;

	FCriticalSection CriticalSection;
};