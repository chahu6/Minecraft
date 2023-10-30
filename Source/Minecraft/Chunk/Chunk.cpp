
#include "Chunk.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 x = -7; x <= 8; ++x)
	{
		for (int32 y = -7; y <= 8; ++y)
		{
			float LocationX = x * BlockSize - 50.0f;
			float LocationY = y * BlockSize - 50.0f;
			UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
			StaticMeshComponent->SetupAttachment(RootComponent);
			StaticMeshComponent->SetRelativeLocation({ LocationX, LocationY, 0.0f });
			UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
			if (StaticMesh)
			{
				StaticMeshComponent->SetStaticMesh(StaticMesh);
			}
			StaticMeshComponent->RegisterComponent();
		}
	}
}

void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

