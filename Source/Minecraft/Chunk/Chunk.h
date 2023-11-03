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
	Dirt,
	Grass,
	Stone,
	Gravel,

	EBT_MAX
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
	void BuildFace(EFaceType FaceType, const FVector& Center);
	void CollectBlocksType(float rangeMin, float rangeMax, float inFactor);
	void GenerateBlock();
	void DrawBlock();

	bool IsCreateFaceInDirection(EFaceType FaceType, int32 X, int32 Y, int32 Z);
private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProduralMesh;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInstance* MaterialInstance;
private:
	float BlockSize = 100.0f;

	TArray<EBlockType> Blocks;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;
};
