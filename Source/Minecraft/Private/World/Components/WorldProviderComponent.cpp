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
	HandleLoadChunks();
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

void UWorldProviderComponent::HandleLoadChunks()
{
	TArray<FChunkPos> GenerateChunksPos;

	TSet<FChunkPos> LastChunksPos;
	Chunks.GetKeys(LastChunksPos);

	int32 CurrentRadius = 0;
	const FChunkPos CenterChunkPos = WorldManager->GetCenterChunkPos();

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

	if (GenerateChunksPos.Num() > 0)
	{
		ParallelFor(GenerateChunksPos.Num(), [this, &GenerateChunksPos](int32 Index)
		{
			WorldManager->GetTerrain()->Generate(WorldManager.Get(), GenerateChunksPos[Index]);
		});
	}

	WorldManager->UnloadChunks(LastChunksPos);
	WorldManager->LoadChunks(GenerateChunksPos);
}

bool UWorldProviderComponent::SpawnChunk(const FChunkPos& ChunkPos)
{
	if (AChunk* Chunk = WorldManager->SpawnChunk(ChunkPos))
	{
		TSharedPtr<FChunkData> ChunkData = MakeShared<FChunkData>();
		Chunk->SetChunkData(ChunkData.ToSharedRef());

		Chunks.Add(ChunkPos, Chunk);
		WorldManager->WorldInfo.Add(ChunkPos, ChunkData);

		Chunk->SetChunkPos(ChunkPos);
		Chunk->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
		return true;
	}

	return false;
}
