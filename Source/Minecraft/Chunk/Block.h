#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Block.generated.h"

enum class EFaceType : uint8;

UCLASS()
class MINECRAFT_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlock();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void BuildBlock();
	void BuildFace(EFaceType faceType, FVector center, FVector up, FVector right, bool reversed);

private:
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* ProduralMesh;

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0; 
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;
};
