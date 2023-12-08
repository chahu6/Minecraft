#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveComponent.generated.h"

USTRUCT()
struct FBlockData
{
	GENERATED_USTRUCT_BODY()

	uint8 BlockID;
	FVector Normal;
	int32 BlockIndex;
	FVector VoxelLocalPosition; // Chunk下的相对坐标
	FVector VoxelWorldPosition; // 体素坐标下的世界坐标，没有乘以BlockSize
	FVector ChunkVoexlWorldPosition;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AMCPlayer;
public:	
	UInteractiveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void AddBlock();
	void RemoveBlock();

	void RayCast();

	uint8 GetBlockID(const FVector& VoxelWorldPosition, FBlockData& OutBlockData);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z);

	void Rebuild_Adjacent_Chunks();

private:
	UPROPERTY()
	APawn* Player;

	UPROPERTY()
	APlayerController* PlayerController;

	FBlockData BlockData;

	FBlockData Temp;

	bool bIsDebug = false;
};
