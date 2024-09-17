#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "World/GenerationMethod.h"
#include "GreedyMeshGenerator.h"
#include "BlockMeshComponent.generated.h"

class AChunk;
class AWorldManager;

UENUM(BlueprintType)
enum class EFaceType : uint8
{
	Forward,
	Background,
	Left,
	Right,
	Up,
	Down
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UBlockMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	UBlockMeshComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void BuildMesh(EGenerationMethod GenerationMethod);

	void Render();

	void Render(const TMap<int32, TSharedPtr<struct FMeshData>>& NewMeshDatas);

private:
	/**
	* 贪心网格化
	*/ 
	void BuildGreedyChunkMesh();

	bool CompareMask(const FMask& M1, const FMask& M2);

	void CreateQuad(const FMask& Mask, const FIntVector& AxisMask, const int32 Width, const int32 Height, const FIntVector& V1, const FIntVector& V2, const FIntVector& V3, const FIntVector& V4);

	/**
	* 普通网格化
	*/
	void BuildChunkMesh();

	bool IsVoid(const FIntVector& BlockWorldVoxelLocation, AWorldManager* WorldManager);

private:
	UPROPERTY()
	TObjectPtr<AChunk> Chunk;

	TMap<int32, TSharedPtr<struct FMeshData>> MeshDatas;
};
