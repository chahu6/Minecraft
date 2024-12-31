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

	WorldManager->UnloadChunks(LastChunksPos);
	WorldManager->LoadChunks(GenerateChunksPos);
}

void UWorldProviderComponent::HandleChunks(TArray<FChunkPos>& GenerateChunksPos, TSet<FChunkPos>& LastChunksPos)
{
	const FChunkPos CenterChunkPos = WorldManager->GetCenterChunkPos();

	TArray<AChunk*> ChunksList;
	for (int32 X = -LoadChunkDistance - 1; X <= LoadChunkDistance + 1; ++X)
	{
		for (int32 Y = -LoadChunkDistance - 1; Y <= LoadChunkDistance + 1; ++Y)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(X, Y);
			if (!Chunks.Contains(ChunkPos))
			{
				if (AChunk* Chunk = SpawnChunk(ChunkPos))
				{
					Chunks.Add(ChunkPos, Chunk);
					ChunksList.Add(Chunk);
					TSharedPtr<FChunkData> ChunkData = MakeShared<FChunkData>();
					Chunk->SetChunkData(ChunkData.ToSharedRef());
					WorldManager->WorldInfo.Add(ChunkPos, ChunkData);
				}
			}
		}
	}

	for (int32 X = -UnLoadChunkDistance; X <= UnLoadChunkDistance; ++X)
	{
		for (int32 Y = -UnLoadChunkDistance; Y <= UnLoadChunkDistance; ++Y)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(X, Y);
			if (LastChunksPos.Contains(ChunkPos))
			{
				LastChunksPos.Remove(ChunkPos);
			}
		}
	}

	if (ChunksList.Num() > 0)
	{
		ParallelFor(ChunksList.Num(), [this, &ChunksList](int32 Index) {
			AChunk* Chunk = ChunksList[Index];
			if (Chunk->GetLoadType() == EChunkLoadType::NotLoad)
			{
				Chunk->SetLoadType(EChunkLoadType::WeakLoaded);
				Chunk->GenerateTerrain(WorldManager->GetTerrain());
			}
		});
	}

	int32 CurrentRadius = 0;
	if (Chunks.Contains(CenterChunkPos) && Chunks[CenterChunkPos]->GetLoadType() == EChunkLoadType::WeakLoaded)
	{
		GenerateChunksPos.Add(CenterChunkPos);
	}
	while (CurrentRadius <= LoadChunkDistance)
	{
		// Forward
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(i, CurrentRadius);
			if (Chunks.Contains(ChunkPos) && Chunks[ChunkPos]->GetLoadType() == EChunkLoadType::WeakLoaded)
			{
				GenerateChunksPos.Add(ChunkPos);
			}
		}

		// Right
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(CurrentRadius, i);
			if (Chunks.Contains(ChunkPos) && Chunks[ChunkPos]->GetLoadType() == EChunkLoadType::WeakLoaded)
			{
				GenerateChunksPos.Add(ChunkPos);
			}
		}

		// Backward
		for (int32 i = -CurrentRadius + 1; i <= CurrentRadius; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(i, -CurrentRadius);
			if (Chunks.Contains(ChunkPos) && Chunks[ChunkPos]->GetLoadType() == EChunkLoadType::WeakLoaded)
			{
				GenerateChunksPos.Add(ChunkPos);
			}
		}

		// Left
		for (int32 i = -CurrentRadius; i <= CurrentRadius - 1; ++i)
		{
			const FChunkPos ChunkPos = CenterChunkPos + FChunkPos(-CurrentRadius, i);
			if (Chunks.Contains(ChunkPos) && Chunks[ChunkPos]->GetLoadType() == EChunkLoadType::WeakLoaded)
			{
				GenerateChunksPos.Add(ChunkPos);
			}
		}

		CurrentRadius++;
	}

	for (const FChunkPos& ChunkPos : GenerateChunksPos)
	{
		if (Chunks.Contains(ChunkPos))
		{
			Chunks[ChunkPos]->SetLoadType(EChunkLoadType::StrongLoaded);
			Chunks[ChunkPos]->GenerateBiome(WorldManager->GetTerrain());
		}
	}
}

AChunk* UWorldProviderComponent::SpawnChunk(const FChunkPos& ChunkPos)
{
	if (AChunk* Chunk = WorldManager->SpawnChunk(ChunkPos))
	{
		Chunk->SetChunkPos(ChunkPos);
		Chunk->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
		Chunk->SetInUse(true);
		return Chunk;
	}

	return nullptr;
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
