// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Components/ChunkTaskPoolComponent.h"
#include "World/WorldManager.h"

UChunkTaskPoolComponent::UChunkTaskPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChunkTaskPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
	ChunkTaskPool = FQueuedThreadPool::Allocate();
	ChunkTaskPool->Create(Concurrency, EThreadPriority::TPri_Highest);
}

void UChunkTaskPoolComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ChunkTaskPool)
	{
		delete ChunkTaskPool;
		ChunkTaskPool = nullptr;
	}
}