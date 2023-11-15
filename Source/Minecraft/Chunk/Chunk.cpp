#include "Chunk.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/Save/ChunkSaveGame.h"
#include "ChunkMeshComponent.h"
#include "Minecraft/MinecraftType/WorldConstants.h"
#include "Minecraft/WorldManager/WorldManager.h"
#include "Minecraft/Utils/Utils.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChunkMesh = CreateDefaultSubobject<UChunkMeshComponent>(TEXT("ChunkMesh"));

	Blocks.Init(EBlockType::Air, CHUNK);
}

void AChunk::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AWorldManager* WorldManager = Cast<AWorldManager>(GetOwner());
	if (WorldManager)
	{
		Seed = WorldManager->GetSeed();
	}
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunk::Load(ITerrainGenerator* Generator)
{
	if (HasLoaded()) return;

	Generator->GenerateChunk(this);
	bIsChunkLoaded = true;
}

void AChunk::DrawChunk()
{
	if (!bIsRendering)
	{
		bIsRendering = true;
		ChunkMesh->RenderingBlock();
	}
}

void AChunk::SetBlock(int32 X, int32 Y, int32 Z, EBlockType BlockType)
{
	Blocks[GetBlocksIndex(X, Y, Z)] = BlockType;
}

EBlockType AChunk::GetBlock(int32 X, int32 Y, int32 Z)
{
	UE_LOG(LogTemp, Warning, TEXT("X = %d, Y = %d, Z = %d"), X, Y, Z);
	return Blocks[GetBlocksIndex(X, Y, Z)];
}
