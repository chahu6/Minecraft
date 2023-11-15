#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Minecraft/MinecraftType/FaceType.h"
#include "ProceduralMeshComponent.h"
#include "Engine/DataTable.h"
#include "ChunkMeshComponent.generated.h"

USTRUCT(BlueprintType)
struct FMeshData
{
	GENERATED_USTRUCT_BODY();

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;
};

USTRUCT(BlueprintType)
struct FBlockInfoTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UMaterialInterface* Material;
};

class AChunk;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UChunkMeshComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChunkMeshComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void RenderingBlock();

	void DrawBlock();

	// Éú³ÉBlock
	void GenerateBlocks();
	void BuildBlock(int32 X, int32 Y, int32 Z);
	void BuildFace(EFaceType FaceType, const FVector& Center, uint8 BlockID = 0);
	bool IsCreateFaceInDirection(EFaceType FaceType, int32 X, int32 Y, int32 Z);
	bool IsCreateFaceInWorld(int32 X, int32 Y, int32 Z);

private:
	bool OutOfBound(int32 X) const noexcept;

private:
	FBlockInfoTableRow* GetBlockInfo(uint8 BlockID);

private:
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* ProduralMesh;

private:
	AChunk* Chunk = nullptr;
	TMap<uint8, FMeshData> MeshDatas;
};
