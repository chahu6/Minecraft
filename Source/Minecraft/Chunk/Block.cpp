#include "Block.h"
#include "Chunk.h"

ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = true;

	ProduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProduralMesh"));
	RootComponent = ProduralMesh;

}

void ABlock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuildBlock();
}

void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlock::BuildBlock()
{
	BuildFace(EFaceType::Up, FVector(0.0f, 0.0f, 50.0f), FVector::ForwardVector, FVector::RightVector, false);
	BuildFace(EFaceType::Down, FVector(0.0f, 0.0f, -50.0f), FVector::ForwardVector, FVector::RightVector, true);
	BuildFace(EFaceType::Forward, FVector(50.0f, 0.0f, 0.0f), FVector::UpVector, FVector::RightVector, true);
	BuildFace(EFaceType::BackGround, FVector(-50.0f, 0.0f, 0.0f), FVector::UpVector, FVector::RightVector, false);
	BuildFace(EFaceType::Right, FVector(0.0f, 50.0f, 0.0f), FVector::UpVector, FVector::ForwardVector, false);
	BuildFace(EFaceType::Left, FVector(0.0f, -50.0f, 0.0f), FVector::UpVector, FVector::ForwardVector, true);

	ProduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}

void ABlock::BuildFace(EFaceType faceType, FVector center, FVector up, FVector right, bool reversed)
{
	int32 index = Vertices.Num();

	Vertices.Add(center - up * 50.0f + right * 50.0f);
	Vertices.Add(center - up * 50.0f - right * 50.0f);
	Vertices.Add(center + up * 50.0f - right * 50.0f);
	Vertices.Add(center + up * 50.0f + right * 50.0f);

	if (reversed)
	{
		Triangles.Add(index + 0);
		Triangles.Add(index + 1);
		Triangles.Add(index + 2);
		Triangles.Add(index + 2);
		Triangles.Add(index + 3);
		Triangles.Add(index + 0);
	}
	else
	{
		Triangles.Add(index + 1);
		Triangles.Add(index + 0);
		Triangles.Add(index + 2);
		Triangles.Add(index + 3);
		Triangles.Add(index + 2);
		Triangles.Add(index + 0);
	}

	const FVector2D bUVs[] = { FVector2D(0.0,0.0),FVector2D(0.0,1.0) ,FVector2D(1.0,1.0) ,FVector2D(1.0,0.0) };
	UV0.Append(bUVs, sizeof(bUVs) / sizeof(bUVs[0]));
}