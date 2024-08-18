#include "World/Components/ChunkManagerComponent.h"
#include "Chunk/Chunk.h"
#include "World/WorldSettings.h"
#include "World/WorldManager.h"

UChunkManagerComponent::UChunkManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChunkManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
}

AChunk* UChunkManagerComponent::GetChunk(const FVector2D& Key)
{
	if (AllChunks.Contains(Key))
	{
		return AllChunks[Key];
	}
	return nullptr;
}

bool UChunkManagerComponent::CreateChunk(const FVector2D& ChunkVoxelPosition)
{
	AChunk* Chunk = GetChunk(ChunkVoxelPosition);
	if (Chunk == nullptr)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = WorldManager;
			Chunk = World->SpawnActorDeferred<AChunk>(AChunk::StaticClass(), FTransform(FVector(ChunkVoxelPosition.X * ChunkSize, ChunkVoxelPosition.Y * ChunkSize, 0.0)), WorldManager);
			Chunk->SetGenerationMethod(WorldManager->ChunkGenerationMethod);
			Chunk->FinishSpawning({}, true);
			AllChunks.Emplace(ChunkVoxelPosition, Chunk);

			// �����µ�����ʱ������������ĸ��������Ҳ����
			Rebuild_Adjacent_Chunks(ChunkVoxelPosition);
			return true;
		}
	}
	return false;
}

void UChunkManagerComponent::Rebuild_Adjacent_Chunks(const FVector2D& ChunkVoxelWorldPosition)
{
	// X��
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X - 1, ChunkVoxelWorldPosition.Y);
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X + 1, ChunkVoxelWorldPosition.Y);

	// Y��
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X, ChunkVoxelWorldPosition.Y - 1);
	Rebuild_Adj_Chunk(ChunkVoxelWorldPosition.X, ChunkVoxelWorldPosition.Y + 1);

	//Z��Ĳ���Ҫ
}

void UChunkManagerComponent::Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y)
{
	AChunk* Chunk = GetChunk(FVector2D(Chunk_World_X, Chunk_World_Y));

	if (Chunk == nullptr) return;

	Chunk->Dirty();
}
