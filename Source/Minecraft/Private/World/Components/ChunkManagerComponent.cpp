#include "World/Components/ChunkManagerComponent.h"
#include "World/WorldManager.h"

UChunkManagerComponent::UChunkManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChunkManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
}