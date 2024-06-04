#pragma once

#include "CoreMinimal.h"
#include "World/WorldSettings.h"
#include "BlockPos.generated.h"

USTRUCT(BlueprintType)
struct FBlockPos
{
	GENERATED_USTRUCT_BODY()

	float X_OFFSET;
	float Y_OFFSET;
	float Z_OFFSET;

	float X_VOXEL_WORLD;
	float Y_VOXEL_WORLD;
	float Z_VOXEL_WORLD;

	explicit FBlockPos()
	{
		Init();
	}

	FORCEINLINE void Init()
	{
		FMemory::Memzero(this, sizeof(FBlockPos));
	}

	FORCEINLINE bool operator==(const FBlockPos& BlockPos)
	{
		return this->X_VOXEL_WORLD == BlockPos.X_VOXEL_WORLD && this->Y_VOXEL_WORLD == BlockPos.Y_VOXEL_WORLD && this->Z_VOXEL_WORLD == BlockPos.Z_VOXEL_WORLD;
	}

	FORCEINLINE FVector VoxelWorldLocation() const
	{
		return FVector(X_VOXEL_WORLD, Y_VOXEL_WORLD, Z_VOXEL_WORLD);
	}

	FORCEINLINE FVector WorldLocation() const
	{
		return FVector(X_VOXEL_WORLD, Y_VOXEL_WORLD, Z_VOXEL_WORLD) * BlockSize;
	}

	FORCEINLINE FVector OffsetLocation() const
	{
		return FVector(X_OFFSET, Y_OFFSET, Z_OFFSET);
	}

	FORCEINLINE void SetOffsetLocation(int32 X, int32 Y, int32 Z)
	{
		X_OFFSET = X;
		Y_OFFSET = Y;
		Z_OFFSET = Z;
	}

	FORCEINLINE void SetVoxelWorldLocation(int32 X, int32 Y, int32 Z)
	{
		X_VOXEL_WORLD = X;
		Y_VOXEL_WORLD = Y;
		Z_VOXEL_WORLD = Z;
	}

	FORCEINLINE FVector ChunkVoexlWorldPosition()
	{
		return FVector();
	}
};