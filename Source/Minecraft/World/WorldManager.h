#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldManager.generated.h"

class AChunk;
class UChunkManagerComponent;

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

	// Key是Chunk在Voxel World的位置，没有乘以ChunkSize的位置
	AChunk* GetChunk(const FVector& ChunkVoxelPosition);

private:
	bool UpdatePosition();

	void AddChunk();

	void RemoveChunk();

	void BuildChunks();

	void RenderChunks();

private:
	UPROPERTY(VisibleAnywhere)
	UChunkManagerComponent* ChunkManager;

	UPROPERTY(EditAnywhere)
	int32 ChunkRenderingRange = 5;

	FVector2D CharacterPosition;
};
