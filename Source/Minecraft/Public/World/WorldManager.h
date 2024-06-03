#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldManager.generated.h"

class AChunk;
class AChunkSection;
class UChunkManagerComponent;
struct FBlockPos;

UCLASS()
class MINECRAFT_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Key��Chunk��Voxel World��λ�ã�û�г���ChunkSize��λ��
	AChunk* GetChunk(const FVector2D& ChunkVoxelPosition);

	AChunkSection* GetChunkSection(const FVector& ChunkVoxelPosition);

	AChunkSection* GetChunkSection(const FBlockPos& BlockPos);

private:
	void InitialWorldChunkLoad();

	bool UpdatePosition();

	void AddChunk();

	void RemoveChunk();

	void RenderChunks();
public:
	TQueue<AChunk*, EQueueMode::Mpsc> TaskQueue;

private:
	UPROPERTY(VisibleAnywhere)
	UChunkManagerComponent* ChunkManager;

	UPROPERTY(EditAnywhere)
	int32 ChunkRenderingRange = 8;

	UPROPERTY(EditAnywhere)
	int32 Seed = 0;

	FVector2D CharacterPosition;

};