#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ChunkMeshComponent.generated.h"

class AChunkSection;

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UChunkMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	UChunkMeshComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void Render();

	void BuildMesh();

	void ClearMeshData();

private:
	UPROPERTY()
	TObjectPtr<AChunkSection> ChunkSection;

	TMap<int32, FMeshData> MeshDatas;
};
