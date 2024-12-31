#pragma once

#include "World/Data/FaceType.h"
#include "GameplayTagContainer.h"

class GlobalInfo;
struct FMeshData;
struct FChunkPos;
struct FBlockPos;

struct FMask
{
	FGameplayTag BlockID;
	int8 Normal = 0;
};

struct FQuadInfo
{
	FVector Pos = FVector::ZeroVector;
	FGameplayTag BlockID;
};

class GreedyMeshGenerator
{
public:
	static void BuildGreedyChunkMesh(GlobalInfo& WorldInfo, const FChunkPos& InChunkPos);
	static void BuildChunkMesh(GlobalInfo& WorldInfo, const FChunkPos& InChunkPos);

private:
	static bool CompareMask(const FMask& M1, const FMask& M2);

	static void CreateQuad(const FMask& Mask, const FIntVector& AxisMask, const int32 Width, const int32 Height, const FIntVector& V1, const FIntVector& V2, const FIntVector& V3, const FIntVector& V4, TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDatas);

	static void CreateBlock(const TArray<FQuadInfo>& TranslucentBlocks, TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDatas);

	static void CreatePlant(const TArray<FQuadInfo>& PlantBlocks, TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDatas);

	static void BuildQuad(const FVector& Pos, const TSharedRef<FMeshData> MeshData);

	static void BuildFace(EFaceType FaceType, const FVector& InBlockPos, const TSharedRef<FMeshData> MeshData);

	static bool IsRender(EFaceType FaceType, GlobalInfo& WorldInfo, const FBlockPos& InBlockPos);

private:
	GreedyMeshGenerator() = delete;
	~GreedyMeshGenerator() = delete;
};