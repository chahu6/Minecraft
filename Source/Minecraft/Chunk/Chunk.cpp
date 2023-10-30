
#include "Chunk.h"
#include "Components/StaticMeshComponent.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeMeshAsset.Succeeded())
	{
		CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
	}
	CubeMesh->SetRelativeScale3D({ 16.0f, 16.0f, 1.0f });
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

