// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ObjectPoolComponent.h"
#include "PooledActor.h"

UObjectPoolComponent::UObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePool();
}

void UObjectPoolComponent::InitializePool()
{
	for (int32 i = 0; i < PoolSize; ++i)
	{
		SpawnPooledActor();
	}
}

APooledActor* UObjectPoolComponent::SpawnPooledActor()
{
	APooledActor* PooledActor = GetWorld()->SpawnActorDeferred<APooledActor>(PooledActorClass, FTransform(), GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	check(PooledActor);

	PooledActor->SetTimeToLive(TimeToLive);
	PooledActor->FinishSpawning(FTransform(), true);
	ObjectPool.AddUnique(PooledActor);
	PooledActor->SetInUse(false);
	return PooledActor;
}

APooledActor* UObjectPoolComponent::FindFirstAvaliableActor()
{
	for (APooledActor* PooledActor : ObjectPool)
	{
		if (!PooledActor->InUse())
		{
			return PooledActor;
		}
	}
	return nullptr;
}

APooledActor* UObjectPoolComponent::SpawnFromPool(const FTransform& SpawnTransform)
{
	APooledActor* PooledActor = FindFirstAvaliableActor();
	if (IsValid(PooledActor))
	{
		PooledActor->SetActorTransform(SpawnTransform);
		PooledActor->SetInUse(true);
		return PooledActor;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor to Pool"));
	}

	return nullptr;
}
