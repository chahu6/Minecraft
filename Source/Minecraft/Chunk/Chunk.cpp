
#include "Chunk.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SimplexNoiseLibrary.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Block = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Block"));
	Block->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeMeshAsset.Succeeded())
	{
		Block->SetStaticMesh(CubeMeshAsset.Object);
	}
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
	CreateBlock(0, 32);
}

void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunk::CreateBlock(float rangeMin, float rangeMax, float inFactor)
{
	for (int32 x = -7; x <= 8; ++x)
	{
		for (int32 y = -7; y <= 8; ++y)
		{
			float LocationX = x * BlockSize - 50.0f;
			float LocationY = y * BlockSize - 50.0f;

			/*
			int32 random = UKismetMathLibrary::RandomIntegerInRange(0, 2);*/
			FVector ActorLocation = GetActorLocation();

			float Noise_X = LocationX + ActorLocation.X;
			float Noise_Y = LocationY + ActorLocation.Y;
			float LocationZ = USimplexNoiseLibrary::SimplexNoiseInRange2D(Noise_X, Noise_Y, rangeMin, rangeMax, inFactor);
			LocationZ = FMath::Floor(LocationZ);
			LocationZ = LocationZ * BlockSize - 50.0f;

			// 添加实例用的是相对位置
			FTransform transform;
			transform.SetLocation({ LocationX, LocationY, LocationZ });
			Block->AddInstance(transform);
		}
	}
}

