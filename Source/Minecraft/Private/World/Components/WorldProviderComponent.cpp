// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Components/WorldProviderComponent.h"
#include "World/WorldManager.h"
#include "World/WorldGenerator.h"
#include "Chunk/Chunk.h"
#include "World/Data/ChunkData.h"
#include "World/Gen/TerrainBase.h"

UWorldProviderComponent::UWorldProviderComponent()
{
	PrimaryComponentTick.bCanEverTick = false; 

}

void UWorldProviderComponent::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
}

void UWorldProviderComponent::UpdateWorld()
{
	TArray<FChunkPos> GenerateChunksPos;

	TSet<FChunkPos> LastChunksPos;
	Chunks.GetKeys(LastChunksPos);

	HandleChunks(GenerateChunksPos, LastChunksPos);

	HandleChunkDatas(GenerateChunksPos);

	WorldManager->UnloadChunks(LastChunksPos);
	WorldManager->LoadChunks(GenerateChunksPos);
}

void UWorldProviderComponent::HandleChunks(TArray<FChunkPos>& GenerateChunksPos, TSet<FChunkPos>& LastChunksPos)
{
	const FChunkPos CenterChunkPos = WorldManager->GetCenterChunkPos();
	int32 CurrentRadius = 0;

	if (LastChunksPos.Contains(CenterChunkPos))
	{
		LastChunksPos.Remove(CenterChunkPos);
	}

	if (!Chunks.Contains(CenterChunkPos))
	{
		if (SpawnChunk(CenterChunkPos))
		{
			GenerateChunksPos.Add(CenterChunkPos);
		}
	}
	while (CurrentRadius <= LoadChunkDistance)
	{
		// Forward
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(i, CurrentRadius);
			if (!Chunks.Contains(ChunkPos))
			{
				if (SpawnChunk(ChunkPos))
				{
					GenerateChunksPos.Add(ChunkPos);
				}
			}
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		// Right
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(CurrentRadius, i);
			if (!Chunks.Contains(ChunkPos))
			{
				if (SpawnChunk(ChunkPos))
				{
					GenerateChunksPos.Add(ChunkPos);
				}
			}
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		// Backward
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(i, -CurrentRadius);
			if (!Chunks.Contains(ChunkPos))
			{
				if (SpawnChunk(ChunkPos))
				{
					GenerateChunksPos.Add(ChunkPos);
				}
			}
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		// Left
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(-CurrentRadius, i);
			if (!Chunks.Contains(ChunkPos))
			{
				if (SpawnChunk(ChunkPos))
				{
					GenerateChunksPos.Add(ChunkPos);
				}
			}
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		CurrentRadius++;
	}

	CurrentRadius = 0;
	while (CurrentRadius <= UnLoadChunkDistance)
	{
		// Forward
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(i, CurrentRadius);
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		// Right
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(CurrentRadius, i);
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		// Backward
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(i, -CurrentRadius);
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		// Left
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(-CurrentRadius, i);
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}

		CurrentRadius++;
	}
}

void UWorldProviderComponent::HandleChunkDatas(TArray<FChunkPos>& GenerateChunksPos)
{
	const FChunkPos CenterChunkPos = WorldManager->GetCenterChunkPos();

	TArray<FChunkPos> LoadChunkDatasPos;

	TSet<FChunkPos> LastChunkDatasPos;
	WorldManager->WorldInfo.ChunkDataMap.GetKeys(LastChunkDatasPos);

	// 生成基础地形，多生成一层范围
	for (int32 X = -LoadChunkDistance - 1; X <= LoadChunkDistance + 1; ++X)
	{
		for (int32 Y = -LoadChunkDistance - 1; Y <= LoadChunkDistance + 1; ++Y)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(X, Y);
			if (!WorldManager->ContainChunkData(ChunkPos))
			{
				LoadChunkDatasPos.Add(ChunkPos);
			}
			if (LastChunkDatasPos.Contains(ChunkPos))
			{
				LastChunkDatasPos.Remove(ChunkPos);
			}
		}
	}

	for (const FChunkPos& ChunkPos : LastChunkDatasPos)
	{
		WorldManager->WorldInfo.Remove(ChunkPos);
	}

	if (LoadChunkDatasPos.Num() > 0)
	{
		ParallelFor(LoadChunkDatasPos.Num(), [this, &LoadChunkDatasPos](int32 Index)
		{
			TSharedPtr<FChunkData> ChunkData = MakeShared<FChunkData>();
			WorldManager->WorldInfo.Add(LoadChunkDatasPos[Index], ChunkData);
			WorldManager->GetTerrain()->Generate(WorldManager.Get(), LoadChunkDatasPos[Index]);
		});
	}

	// 装饰雕刻
	for (const FChunkPos& LoadChunkPos : GenerateChunksPos)
	{
		WorldManager->GetTerrain()->GenerateBiome(WorldManager.Get(), LoadChunkPos);
	}
}

bool UWorldProviderComponent::SpawnChunk(const FChunkPos& ChunkPos)
{
	if (AChunk* Chunk = WorldManager->SpawnChunk(ChunkPos))
	{
		//Chunk->SetChunkData(ChunkData.ToSharedRef());

		Chunks.Add(ChunkPos, Chunk);

		Chunk->SetChunkPos(ChunkPos);
		Chunk->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
		return true;
	}

	return false;
}

AChunk* UWorldProviderComponent::GetChunk(const FChunkPos& InChunkPos) const
{
	if (Chunks.Contains(InChunkPos))
	{
		return Chunks[InChunkPos];
	}
	return nullptr;
}

void UWorldProviderComponent::RemoveChunk(const FChunkPos& InChunkPos)
{
	if (Chunks.Contains(InChunkPos))
	{
		Chunks[InChunkPos]->SetInUse(false);
		Chunks.Remove(InChunkPos);
	}
}
