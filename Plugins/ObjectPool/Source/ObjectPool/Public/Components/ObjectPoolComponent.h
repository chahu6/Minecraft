// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"

class APooledActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJECTPOOL_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolComponent();

protected:
	virtual void BeginPlay() override;

	void InitializePool();

	APooledActor* SpawnPooledActor();

	template<class T>
	T* SpawnPooledActor()
	{
		return Cast<T>(SpawnPooledActor());
	}

public:	
	UFUNCTION(BlueprintCallable)
	APooledActor* FindFirstAvaliableActor();

	UFUNCTION(BlueprintCallable)
	APooledActor* SpawnFromPool(const FTransform& SpawnTransform);

	template<class T>
	T* SpawnFromPool(const FTransform& SpawnTransform)
	{
		return Cast<T>(SpawnFromPool(SpawnTransform));
	}

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APooledActor> PooledActorClass;

	UPROPERTY(EditAnywhere)
	int32 PoolSize = 12;

	UPROPERTY(EditAnywhere)
	float TimeToLive = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TArray<APooledActor*> ObjectPool;
};
