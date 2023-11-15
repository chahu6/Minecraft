#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Minecraft/MinecraftType/FaceType.h"
#include "Minecraft/Chunk/ChunkManager.h"
#include "Minecraft/MinecraftType/WorldConstants.h"
#include "Minecraft/MinecraftType/BlockType.h"
#include "WorldManager.generated.h"

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

	UFUNCTION(BlueprintPure)
	int32 GetChunkNum() const;

	bool IsCreateFaceInDirection(EFaceType FaceType, const FVector2D& ChunkLocation, int32 X, int32 Y, int32 Z);

	EBlockType GetBlockTypeFromPosition(int32 X, int32 Y, int32 Z);

private:
	bool UpdateLocation();

	void InitialWorldChunkLoad();

	void UpdateChunks();

	void RenderingChunks();

private:
	FVector2D ChunkLocation = FVector2D::ZeroVector; // 实际是下标

	UPROPERTY(EditAnywhere)
	int32 Seed;

	UPROPERTY(EditAnywhere)
	int32 RenderingChunkScale = 3;

	UPROPERTY(EditAnywhere)
	int32 RemoveSizeScale = 3;

	float RenderingRange;

	FChunkManager ChunkManager;

public:
	FORCEINLINE int32 GetSeed() const { return Seed; }
};
