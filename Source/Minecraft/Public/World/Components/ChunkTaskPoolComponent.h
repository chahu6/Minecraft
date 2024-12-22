// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChunkTaskPoolComponent.generated.h"

class AWorldManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UChunkTaskPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChunkTaskPoolComponent();

	FQueuedThreadPool* ChunkTaskPool;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category = "Multi-Threading")
	uint8 Concurrency = 6;
		
private:
	TWeakObjectPtr<AWorldManager> WorldManager;

};
