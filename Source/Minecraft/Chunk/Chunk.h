#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Chunk.generated.h"

UENUM(BlueprintType)
enum class EFaceType : uint8
{
	Forward,
	BackGround,
	Left,
	Right,
	Up,
	Down,

	EFT_MAX
};

UENUM(BlueprintType)
enum class EBlockType : uint8
{
	None = 0,

	Air,
	Stone,
	Dirt,
	Grass,
	BedRock,

	EBT_MAX
};

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

UCLASS()
class MINECRAFT_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunk();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void BuildChunk(float rangeMin, float rangeMax, float inFactor = 0.0001f);
	void BuildBlock(int32 X, int32 Y, int32 Z);
	void BuildFace(EFaceType FaceType, const FVector& Center, uint8 BlockID = 0);
	void CollectBlocksType(float rangeMin, float rangeMax, float inFactor);
	void GenerateBlock();
	void DrawBlock();

	bool IsCreateFaceInDirection(EFaceType FaceType, int32 X, int32 Y, int32 Z);

	uint8 CalcBlockID(int32 ZValue, int32 MaxZValue);

	FBlockInfoTableRow* GetBlockInfo(uint8 BlockID);
private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProduralMesh;

private:
	float BlockSize = 100.0f;

	UPROPERTY()
	UDataTable* DataTable;

	TArray<EBlockType> Blocks;
	TMap<uint8, FMeshData> MeshDatas;
};
